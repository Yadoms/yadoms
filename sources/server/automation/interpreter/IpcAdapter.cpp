#include "stdafx.h"
#include "IpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "serializers/Information.h"
#include <shared/event/EventHandler.hpp>

namespace automation
{
   namespace interpreter
   {
      const size_t CIpcAdapter::m_maxMessages(100);
      const size_t CIpcAdapter::m_maxMessageSize(100000);

      CIpcAdapter::CIpcAdapter(const std::string& interpreterName,
                               boost::shared_ptr<shared::script::yInterpreterApi::IYInterpreterApi> apiImplementation)
         : m_interpreterName(interpreterName),
           m_apiImplementation(apiImplementation),
           m_id(createId()),
           m_sendMessageQueueId(m_id + ".interpreter_IPC.toInterpreter"),
           m_receiveMessageQueueId(m_id + ".interpreter_IPC.toYadoms"),
           m_sendMessageQueueRemover(m_sendMessageQueueId),
           m_receiveMessageQueueRemover(m_receiveMessageQueueId),
           m_sendMessageQueue(boost::interprocess::create_only, m_sendMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
           m_receiveMessageQueue(boost::interprocess::create_only, m_receiveMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
           m_sendBuffer(boost::make_shared<unsigned char[]>(m_sendMessageQueue.get_max_msg_size())),
           m_messageQueueReceiveThread(boost::thread(&CIpcAdapter::messageQueueReceiveThreaded, this, shared::CLog::getCurrentThreadName()))
      {
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
         ss << "yInterpreter." << boost::uuids::random_generator()();
         return ss.str();
      }

      void CIpcAdapter::messageQueueReceiveThreaded(const std::string& logName)
      {
         // Verify that the version of the library that we linked against is
         // compatible with the version of the headers we compiled against.
         GOOGLE_PROTOBUF_VERIFY_VERSION;

         YADOMS_LOG_CONFIGURE(logName);
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

      void CIpcAdapter::send(const interpreter_IPC::toInterpreter::msg& pbMsg)
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

         YADOMS_LOG(trace) << "[SEND] message " << pbMsg.OneOf_case() << " to interpreter " << m_interpreterName;

         m_sendMessageQueue.send(m_sendBuffer.get(), pbMsg.GetCachedSize(), 0);
      }

      void CIpcAdapter::send(const interpreter_IPC::toInterpreter::msg& pbMsg,
                             boost::function1<bool, const interpreter_IPC::toYadoms::msg&> checkExpectedMessageFunction,
                             boost::function1<void, const interpreter_IPC::toYadoms::msg&> onReceiveFunction,
                             const boost::posix_time::time_duration& timeout)
      {
         shared::event::CEventHandler receivedEvtHandler;

         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook = [&](const interpreter_IPC::toYadoms::msg& receivedMsg)-> bool
               {
                  if (!checkExpectedMessageFunction(receivedMsg))
                     return false;

                  receivedEvtHandler.postEvent<const interpreter_IPC::toYadoms::msg>(shared::event::kUserFirstId, receivedMsg);
                  return true;
               };
         }

         send(pbMsg);

         if (receivedEvtHandler.waitForEvents(timeout) == shared::event::kTimeout)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error((boost::format("No answer from interpreter when sending message %1%") % pbMsg.OneOf_case()).str());
         }

         onReceiveFunction(receivedEvtHandler.getEventData<const interpreter_IPC::toYadoms::msg>());
      }

      void CIpcAdapter::processMessage(boost::shared_ptr<const unsigned char[]> message, size_t messageSize)
      {
         if (messageSize < 1)
            throw shared::exception::CInvalidParameter("messageSize");

         // Unserialize message
         interpreter_IPC::toYadoms::msg toYadomsProtoBuffer;
         if (!toYadomsProtoBuffer.ParseFromArray(message.get(), messageSize))
            throw shared::exception::CInvalidParameter("message : fail to parse received data into protobuf format");

         YADOMS_LOG(trace) << "[RECEIVE] message " << toYadomsProtoBuffer.OneOf_case() << " from interpreter " << m_interpreterName << (m_onReceiveHook ? " (onReceiveHook ENABLED)" : "");

         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            if (m_onReceiveHook && m_onReceiveHook(toYadomsProtoBuffer))
            {
               m_onReceiveHook.clear();
               return;
            }
         }

         // Process message
         switch (toYadomsProtoBuffer.OneOf_case())
         {
         case interpreter_IPC::toYadoms::msg::kNotifiyScriptStopped: processNotifiyScriptStopped(toYadomsProtoBuffer.notifiyscriptstopped());
            break;
         default:
            throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % toYadomsProtoBuffer.OneOf_case()).str());
         }
      }

      void CIpcAdapter::processNotifiyScriptStopped(const interpreter_IPC::toYadoms::NotifiyScriptStopped& notifiyScriptStopped) const
      {
         m_apiImplementation->notifyScriptStopped(notifiyScriptStopped.scriptinstanceid(),
                                                  notifiyScriptStopped.error());
      }

      void CIpcAdapter::postStopRequest()
      {
         interpreter_IPC::toInterpreter::msg msg;
         auto message = msg.mutable_system();
         message->set_type(interpreter_IPC::toInterpreter::System_EventType_kRequestStop);

         send(msg);
      }

      void CIpcAdapter::postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information,
                                 const boost::filesystem::path& logFile,
                                 const std::string& logLevel)
      {
         interpreter_IPC::toInterpreter::msg msg;
         auto message = msg.mutable_init();
         serializers::CInformation(information).toPb(message->mutable_interpreterinformation());
         message->set_logfile(logFile.string());
         message->set_loglevel(logLevel);

         send(msg);
      }

      void CIpcAdapter::postAvalaibleRequest(boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request)
      {
         interpreter_IPC::toInterpreter::msg req;
         req.mutable_avalaiblerequest();
         auto avalaible = false;

         try
         {
            send(req,
                 [&](const interpreter_IPC::toYadoms::msg& ans) -> bool
                 {
                    return ans.has_avalaibleanswer();
                 },
                 [&](const interpreter_IPC::toYadoms::msg& ans) -> void
                 {
                    avalaible = ans.avalaibleanswer().avalaible();
                 });
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Interpreter doesn't answer to avalaible request : %1%") % e.what()).str());
            return;
         }

         request->sendSuccess(avalaible);
      }

      void CIpcAdapter::postLoadScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request)
      {
         interpreter_IPC::toInterpreter::msg req;
         auto message = req.mutable_loadscriptcontentrequest();
         message->set_scriptpath(request->getScriptPath());
         std::string content;

         try
         {
            send(req,
                 [&](const interpreter_IPC::toYadoms::msg& ans) -> bool
                 {
                    return ans.has_loadscriptcontentanswer();
                 },
                 [&](const interpreter_IPC::toYadoms::msg& ans) -> void
                 {
                    content = ans.loadscriptcontentanswer().content();
                 });
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Interpreter doesn't answer to load script content request : %1%") % e.what()).str());
            return;
         }

         request->sendSuccess(content);
      }

      void CIpcAdapter::postSaveScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> request)
      {
         interpreter_IPC::toInterpreter::msg req;
         auto message = req.mutable_savescriptcontentrequest();
         message->set_scriptpath(request->getScriptPath());
         message->set_content(request->getScriptContent());

         try
         {
            send(req,
                 [&](const interpreter_IPC::toYadoms::msg& ans) -> bool
                 {
                    return ans.has_savescriptcontentanswer();
                 },
                 [&](const interpreter_IPC::toYadoms::msg& ans) -> void
                 {
                 });
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Interpreter doesn't answer to save script content request : %1%") % e.what()).str());
            return;
         }

         request->sendSuccess();
      }

      void CIpcAdapter::postStartScript(boost::shared_ptr<shared::script::yInterpreterApi::IStartScript> request)
      {
         interpreter_IPC::toInterpreter::msg req;
         auto message = req.mutable_startscript();
         message->set_scriptinstanceid(request->getScriptInstanceId());
         message->set_scriptpath(request->getScriptPath().string());
         message->set_scriptapiid(request->getScriptApiId());
         message->set_scriptlogpath(request->getScriptLogPath().string());
         std::string scriptProcessId;

         send(req);
      }

      void CIpcAdapter::postStopScript(boost::shared_ptr<shared::script::yInterpreterApi::IStopScript> request)
      {
         interpreter_IPC::toInterpreter::msg req;
         auto message = req.mutable_stopscript();
         message->set_scriptinstanceid(request->getScriptInstanceId());
         std::string content;

         send(req);
      }
   }
} // namespace automation::interpreter


