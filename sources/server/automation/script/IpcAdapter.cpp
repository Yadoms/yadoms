#include "stdafx.h"
#include "IpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/event/EventHandler.hpp>

namespace automation
{
   namespace script
   {
      const size_t CIpcAdapter::m_maxMessages(100);
      const size_t CIpcAdapter::m_maxMessageSize(100000);

      CIpcAdapter::CIpcAdapter(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                               int ruleId)
         : m_scriptApi(yScriptApi),
           m_id(createId()),
           m_sendMessageQueueId(m_id + ".script_IPC.toScript"),
           m_receiveMessageQueueId(m_id + ".script_IPC.toYadoms"),
           m_sendMessageQueueRemover(m_sendMessageQueueId),
           m_receiveMessageQueueRemover(m_receiveMessageQueueId),
           m_sendMessageQueue(boost::interprocess::create_only, m_sendMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
           m_receiveMessageQueue(boost::interprocess::create_only, m_receiveMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
           m_sendBuffer(boost::make_shared<unsigned char[]>(m_sendMessageQueue.get_max_msg_size())),
           m_messageQueueReceiveThread(boost::thread(&CIpcAdapter::messageQueueReceiveThreaded, this))
      {
         YADOMS_LOG_CONFIGURE("automation.script.IpcAdapter#" + std::to_string(ruleId));
      }

      CIpcAdapter::~CIpcAdapter()
      {
         m_messageQueueReceiveThread.interrupt();
         m_messageQueueReceiveThread.join();
      }

      std::string CIpcAdapter::id() const
      {
         return m_id;
      }

      std::string CIpcAdapter::createId()
      {
         std::stringstream ss;
         ss << "yScript." << boost::uuids::random_generator()();
         return ss.str();
      }

      void CIpcAdapter::messageQueueReceiveThreaded()
      {
         // Verify that the version of the library that we linked against is
         // compatible with the version of the headers we compiled against.
         GOOGLE_PROTOBUF_VERIFY_VERSION;

         YADOMS_LOG(information) << "Message queue ID : " << m_id;

         try
         {
            auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue.get_max_msg_size()));
            size_t messageSize;
            unsigned int messagePriority;
            while (true)
            {
               try
               {
                  // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
                  // polling and call boost::this_thread::interruption_point to exit properly
                  // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
                  auto messageWasReceived = m_receiveMessageQueue.timed_receive(message.get(),
                                                                                m_receiveMessageQueue.get_max_msg_size(),
                                                                                messageSize,
                                                                                messagePriority,
                                                                                boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1));
                  boost::this_thread::interruption_point();

                  if (messageWasReceived)
                     processMessage(message, messageSize);
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Error receiving/processing queue message : " << ex.what();
               }
            }
         }
         catch (boost::interprocess::interprocess_exception& ex)
         {
            YADOMS_LOG(error) << "Error creating/using message queues (" << m_id << ") in context accessor : " << ex.what();
         }
         catch (boost::thread_interrupted&)
         {
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << ex.what();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown error";
         }

         YADOMS_LOG(debug) << "Close message queues";

         // Delete all global objects allocated by libprotobuf.
         google::protobuf::ShutdownProtobufLibrary();
      }

      void CIpcAdapter::send(const script_IPC::toScript::msg& pbMsg)
      {
         if (!pbMsg.IsInitialized())
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : message is not fully initialized ==> ignored)";
            return;
         }

         if (pbMsg.ByteSize() > static_cast<int>(m_sendMessageQueue.get_max_msg_size()))
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : message is too big (" << pbMsg.ByteSize() << " bytes) ==> ignored)";
            return;
         }

         boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);
         if (!pbMsg.SerializeToArray(m_sendBuffer.get(), pbMsg.GetCachedSize()))
         {
            YADOMS_LOG(error) << "CIpcAdapter::send : fail to serialize message (too big ?) ==> ignored)";
            return;
         }

         YADOMS_LOG(trace) << "[SEND] message " << pbMsg.OneOf_case();

         m_sendMessageQueue.send(m_sendBuffer.get(), pbMsg.GetCachedSize(), 0);
      }

      void CIpcAdapter::send(const script_IPC::toScript::msg& pbMsg,
                             boost::function1<bool, const script_IPC::toYadoms::msg&> checkExpectedMessageFunction,
                             boost::function1<void, const script_IPC::toYadoms::msg&> onReceiveFunction,
                             const boost::posix_time::time_duration& timeout)
      {
         shared::event::CEventHandler receivedEvtHandler;

         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook = [&](const script_IPC::toYadoms::msg& receivedMsg)-> bool
               {
                  if (!checkExpectedMessageFunction(receivedMsg))
                     return false;

                  receivedEvtHandler.postEvent<const script_IPC::toYadoms::msg>(shared::event::kUserFirstId, receivedMsg);
                  return true;
               };
         }

         send(pbMsg);

         if (receivedEvtHandler.waitForEvents(timeout) == shared::event::kTimeout)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error((boost::format("No answer from script when sending message %1%") % pbMsg.OneOf_case()).str());
         }

         onReceiveFunction(receivedEvtHandler.getEventData<const script_IPC::toYadoms::msg>());
      }

      void CIpcAdapter::processMessage(boost::shared_ptr<const unsigned char[]> message,
                                       size_t messageSize)
      {
         if (messageSize < 1)
            throw shared::exception::CInvalidParameter("messageSize");

         // Unserialize message
         script_IPC::toYadoms::msg toYadomsProtoBuffer;
         if (!toYadomsProtoBuffer.ParseFromArray(message.get(), messageSize))
            throw shared::exception::CInvalidParameter("message : fail to parse received data into protobuf format");

         YADOMS_LOG(trace) << "[RECEIVE] message " << toYadomsProtoBuffer.OneOf_case() << (m_onReceiveHook ? " (onReceiveHook ENABLED)" : "");

         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            if (m_onReceiveHook && m_onReceiveHook(toYadomsProtoBuffer))
            {
               m_onReceiveHook.clear();
               return;
            }
         }

         switch (toYadomsProtoBuffer.OneOf_case())
         {
         case script_IPC::toYadoms::msg::kGetKeywordId: processGetKeywordId(toYadomsProtoBuffer.getkeywordid());
            break;
         case script_IPC::toYadoms::msg::kGetRecipientId: processGetRecipientId(toYadomsProtoBuffer.getrecipientid());
            break;
         case script_IPC::toYadoms::msg::kReadKeyword: processReadKeyword(toYadomsProtoBuffer.readkeyword());
            break;
         case script_IPC::toYadoms::msg::kWaitForNextAcquisition: processWaitForNextAcquisition(toYadomsProtoBuffer.waitfornextacquisition());
            break;
         case script_IPC::toYadoms::msg::kWaitForNextAcquisitions: processWaitForNextAcquisitions(toYadomsProtoBuffer.waitfornextacquisitions());
            break;
         case script_IPC::toYadoms::msg::kWaitForEvent: processWaitForEvent(toYadomsProtoBuffer.waitforevent());
            break;
         case script_IPC::toYadoms::msg::kGetKeywordsByCapacity: processGetKeywordsByCapacity(toYadomsProtoBuffer.getkeywordsbycapacity());
            break;
         case script_IPC::toYadoms::msg::kWriteKeyword: processWriteKeyword(toYadomsProtoBuffer.writekeyword());
            break;
         case script_IPC::toYadoms::msg::kSendNotification: processSendNotification(toYadomsProtoBuffer.sendnotification());
            break;
         case script_IPC::toYadoms::msg::kGetInfo: processGetInfo(toYadomsProtoBuffer.getinfo());
            break;
         case script_IPC::toYadoms::msg::kGetKeywordName: processGetKeywordName(toYadomsProtoBuffer.getkeywordname());
            break;
         case script_IPC::toYadoms::msg::kGetKeywordDeviceName: processGetKeywordDeviceName(toYadomsProtoBuffer.getkeyworddevicename());
            break;
         default:
            throw std::invalid_argument((boost::format("message : unknown message type %1%") % toYadomsProtoBuffer.OneOf_case()).str());
         }
      }

      void CIpcAdapter::processGetKeywordId(const script_IPC::toYadoms::GetKeywordId& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_getkeywordid();
         try
         {
            answer->set_id(m_scriptApi->getKeywordId(request.devicename(),
                                                     request.keywordname()));
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processGetKeywordId request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processGetRecipientId(const script_IPC::toYadoms::GetRecipientId& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_getrecipientid();
         try
         {
            answer->set_id(m_scriptApi->getRecipientId(request.firstname(),
                                                       request.lastname()));
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processGetRecipientId request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processReadKeyword(const script_IPC::toYadoms::ReadKeyword& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_readkeyword();
         try
         {
            answer->set_value(m_scriptApi->readKeyword(request.keywordid()));
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processReadKeyword request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processWaitForNextAcquisition(const script_IPC::toYadoms::WaitForNextAcquisition& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_waitfornextacquisition();
         try
         {
            answer->set_acquisition(m_scriptApi->waitForNextAcquisition(request.keywordid(),
                                                                        request.timeout()));
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processWaitForNextAcquisition request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processWaitForNextAcquisitions(const script_IPC::toYadoms::WaitForNextAcquisitions& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_waitfornextacquisitions();
         try
         {
            std::vector<int> keywordIdList;
            for (auto it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
               keywordIdList.push_back(*it);
            auto result = m_scriptApi->waitForNextAcquisitions(keywordIdList,
                                                               request.timeout());
            answer->set_keywordid(result.first);
            if (!result.second.empty())
               answer->set_acquisition(result.second);
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processWaitForNextAcquisitions request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processWaitForEvent(const script_IPC::toYadoms::WaitForEvent& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_waitforevent();
         try
         {
            std::vector<int> keywordIdList;
            for (auto it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
               keywordIdList.push_back(*it);

            auto result = m_scriptApi->waitForEvent(keywordIdList,
                                                    request.receivedatetimeevent(),
                                                    request.timeout());

            switch (result.getType())
            {
            case shared::script::yScriptApi::CWaitForEventResult::kTimeout: answer->set_type(script_IPC::toScript::WaitForEvent_EventType_kTimeout);
               break;
            case shared::script::yScriptApi::CWaitForEventResult::kKeyword: answer->set_type(script_IPC::toScript::WaitForEvent_EventType_kKeyword);
               break;
            case shared::script::yScriptApi::CWaitForEventResult::kDateTime: answer->set_type(script_IPC::toScript::WaitForEvent_EventType_kDateTime);
               break;
            default:
               throw shared::exception::CInvalidParameter("CWaitForEventResult::type");
            }
            answer->set_keywordid(result.getKeywordId());
            if (!result.getValue().empty())
               answer->set_acquisition(result.getValue());
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing WaitForEvent request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processGetKeywordsByCapacity(const script_IPC::toYadoms::GetKeywordsByCapacity& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_getkeywordsbycapacity();
         try
         {
            auto keywordIdList = m_scriptApi->getKeywordsByCapacity(request.capacity());
            for (auto i = keywordIdList.begin(); i != keywordIdList.end(); ++i)
               answer->add_keywordids(*i);
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processGetKeywordsByCapacity request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processWriteKeyword(const script_IPC::toYadoms::WriteKeyword& request)
      {
         script_IPC::toScript::msg ans;
         ans.mutable_writekeyword();
         try
         {
            m_scriptApi->writeKeyword(request.keywordid(),
                                      request.newstate());
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processWriteKeyword request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processSendNotification(const script_IPC::toYadoms::SendNotification& request)
      {
         script_IPC::toScript::msg ans;
         ans.mutable_sendnotification();
         try
         {
            m_scriptApi->sendNotification(request.keywordid(),
                                          request.recipientid(),
                                          request.message());
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processSendNotification request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processGetInfo(const script_IPC::toYadoms::GetInfo& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_getinfo();
         try
         {
            shared::script::yScriptApi::IYScriptApi::EInfoKeys key;
            switch (request.key())
            {
            case script_IPC::toYadoms::GetInfo_Key_kSunrise: key = shared::script::yScriptApi::IYScriptApi::kSunrise;
               break;
            case script_IPC::toYadoms::GetInfo_Key_kSunset: key = shared::script::yScriptApi::IYScriptApi::kSunset;
               break;
            case script_IPC::toYadoms::GetInfo_Key_kLatitude: key = shared::script::yScriptApi::IYScriptApi::kLatitude;
               break;
            case script_IPC::toYadoms::GetInfo_Key_kLongitude: key = shared::script::yScriptApi::IYScriptApi::kLongitude;
               break;
            case script_IPC::toYadoms::GetInfo_Key_kAltitude: key = shared::script::yScriptApi::IYScriptApi::kAltitude;
               break;
            case script_IPC::toYadoms::GetInfo_Key_kYadomsServerOS: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerOS;
               break;
            case script_IPC::toYadoms::GetInfo_Key_kYadomsServerVersion: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerVersion;
               break;
            default:
               throw shared::exception::CInvalidParameter("answer.waitforeventrequestanswer.type");
            }

            answer->set_value(m_scriptApi->getInfo(key));
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processGetInfo request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processGetKeywordName(const script_IPC::toYadoms::GetKeywordName& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_getkeywordname();

         try
         {
            answer->set_keywordname(""); //predefine with "", to ensure answer is complete. Even if an error occurs, it should always return ""
            auto name = m_scriptApi->getKeywordName(request.keywordid());
            answer->set_keywordname(name);
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processGetKeywordName request : " << ex.what() << std::endl;
         }

         send(ans);
      }

      void CIpcAdapter::processGetKeywordDeviceName(const script_IPC::toYadoms::GetKeywordDeviceName& request)
      {
         script_IPC::toScript::msg ans;
         auto answer = ans.mutable_getkeyworddevicename();

         try
         {
            answer->set_devicename(""); //predefine with "", to ensure answer is complete. Even if an error occurs, it should always return ""
            auto name = m_scriptApi->getKeywordDeviceName(request.keywordid());
            answer->set_devicename(name);
         }
         catch (std::exception& ex)
         {
            ans.set_error(ex.what());
            std::cout << "Error processing processGetKeywordDeviceName request : " << ex.what() << std::endl;
         }

         send(ans);
      }
   }
} // namespace automation::script


