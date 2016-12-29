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

      CIpcAdapter::CIpcAdapter(const std::string& interpreterName)
         : m_interpreterName(interpreterName),
           m_id(createId()),
           m_sendMessageQueueId(m_id + ".interpreter_IPC.toInterpreter"),
           m_receiveMessageQueueId(m_id + ".interpreter_IPC.toYadoms"),
           m_sendMessageQueueRemover(m_sendMessageQueueId),
           m_receiveMessageQueueRemover(m_receiveMessageQueueId),
           m_sendMessageQueue(boost::interprocess::create_only, m_sendMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
           m_receiveMessageQueue(boost::interprocess::create_only, m_receiveMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
           m_sendBuffer(boost::make_shared<unsigned char[]>(m_sendMessageQueue.get_max_msg_size())),
           m_messageQueueReceiveThread(boost::thread(&CIpcAdapter::messageQueueReceiveThreaded, this))
      {
         YADOMS_LOG_CONFIGURE("automation.interpreter.IpcAdapter");
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
            // No request from interpreter to Yadoms
         default:
            throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % toYadomsProtoBuffer.OneOf_case()).str());
         }
      }

      void CIpcAdapter::postStopRequest()
      {
         interpreter_IPC::toInterpreter::msg msg;
         auto message = msg.mutable_system();
         message->set_type(interpreter_IPC::toInterpreter::System_EventType_kRequestStop);

         send(msg);
      }

      void CIpcAdapter::postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information)
      {
         interpreter_IPC::toInterpreter::msg msg;
         auto message = msg.mutable_init();
         serializers::CInformation(information).toPb(message->mutable_interpreterinformation());

         send(msg);
      }
   }
} // namespace automation::interpreter


