#include "stdafx.h"
#include "ApiImplementation.h"
#include <shared/DataContainer.h>
#include "AvalaibleRequest.h"
#include "LoadScriptContentRequest.h"
#include "SaveScriptContentRequest.h"
#include "Information.h"
#include "StartScript.h"
#include "StopScript.h"
#include "PurgeScriptLog.h"
#include <shared/communication/SmallHeaderMessageCutter.h>


namespace interpreter_cpp_api
{
   CApiImplementation::CApiImplementation()
      : m_initialized(false),
        m_stopRequested(false)
   {
   }

   void CApiImplementation::setSendingMessageQueue(boost::shared_ptr<boost::interprocess::message_queue> sendMessageQueue)
   {
      m_sendMessageQueue = sendMessageQueue;
      m_messageCutter = boost::make_shared<shared::communication::SmallHeaderMessageCutter>(m_sendMessageQueue->get_max_msg_size(),
                                                                                            m_sendMessageQueue->get_max_msg());
   }

   bool CApiImplementation::stopRequested() const
   {
      return m_stopRequested;
   }

   void CApiImplementation::notifyScriptStopped(int scriptInstanceId,
                                                const std::string& error)
   {
      interpreter_IPC::toYadoms::msg req;
      auto request = req.mutable_notifiyscriptstopped();
      request->set_scriptinstanceid(scriptInstanceId);
      request->set_error(error);
      send(req);
   }

   void CApiImplementation::send(const interpreter_IPC::toYadoms::msg& msg) const
   {
      try
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);

         if (!m_sendMessageQueue || !m_messageCutter)
            throw std::runtime_error(
               (boost::format("CApiImplementation::send \"%1%\", interpreter API not ready to send message") % msg.descriptor()->full_name()).str());

         if (!msg.IsInitialized())
            throw std::runtime_error(
               (boost::format("CApiImplementation::send \"%1%\", request is not fully initialized") % msg.descriptor()->full_name()).str());

         const auto pbMessageSize = msg.ByteSizeLong();
         const auto serializedMessage = boost::make_shared<unsigned char[]>(pbMessageSize);
         if (!msg.SerializeWithCachedSizesToArray(serializedMessage.get()))
            throw std::runtime_error(
               (boost::format("CApiImplementation::send \"%1%\", fail to serialize request (too big ?)") % msg.descriptor()->full_name()).str());

         const auto cutMessage = m_messageCutter->cut(serializedMessage,
                                                         pbMessageSize);

         if (!cutMessage->empty())
         {
            for (const auto& part : *cutMessage)
            {
               m_sendMessageQueue->send(part->formattedMessage(),
                                        part->formattedSize(),
                                        0);
            }
         }
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         throw std::runtime_error((boost::format("CApiImplementation::send \"%1%\" : error :%2%") % msg.descriptor()->full_name() % ex.what()).str());
      }
   }

   void CApiImplementation::send(const interpreter_IPC::toYadoms::msg& msg,
                                 boost::function1<bool, const interpreter_IPC::toInterpreter::msg&> checkExpectedMessageFunction,
                                 boost::function1<void, const interpreter_IPC::toInterpreter::msg&> onReceiveFunction) const
   {
      shared::event::CEventHandler receivedEvtHandler;
      enum
      {
         kExpectedEventReceived = shared::event::kUserFirstId,
         kErrorReceived
      };

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         m_onReceiveHook = [&](const interpreter_IPC::toInterpreter::msg& receivedMsg)-> bool
         {
            if (!receivedMsg.error().empty())
            {
               receivedEvtHandler.postEvent<const interpreter_IPC::toInterpreter::msg>(kErrorReceived, receivedMsg);
               return true;
            }

            if (!checkExpectedMessageFunction(receivedMsg))
               return false;

            receivedEvtHandler.postEvent<const interpreter_IPC::toInterpreter::msg>(kExpectedEventReceived, receivedMsg);
            return true;
         };
      }

      send(msg);

      switch (receivedEvtHandler.waitForEvents(boost::posix_time::minutes(1)))
      {
      case kExpectedEventReceived:
         {
            onReceiveFunction(receivedEvtHandler.getEventData<const interpreter_IPC::toInterpreter::msg>());
            break;
         }

      case kErrorReceived:
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error(
               (boost::format("Error \"%1%\" received from Yadoms when sending message %2%") % receivedEvtHandler.getEventData<const interpreter_IPC::
                  toInterpreter::msg>().error() % msg.OneOf_case()).str());
         }

      case shared::event::kTimeout:
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error((boost::format("No answer from Yadoms when sending message %1%") % msg.OneOf_case()).str());
         }

      default:
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error(
               (boost::format("Invalid event received %1% when sending message %2%") % receivedEvtHandler.getEventId() % msg.OneOf_case()).str());
         }
      }
   }

   void CApiImplementation::onReceive(boost::shared_ptr<const unsigned char[]> message,
                                      size_t messageSize)
   {
      if (messageSize < 1)
         throw std::runtime_error("CApiImplementation::onReceive : received Yadoms answer is zero length");

      interpreter_IPC::toInterpreter::msg toInterpreterProtoBuffer;
      if (!toInterpreterProtoBuffer.ParseFromArray(message.get(), messageSize))
         throw shared::exception::CInvalidParameter(
            (boost::format("message : fail to parse received data into protobuf format (received buffer size is %1%)") % messageSize).str());

      if (!m_initialized)
      {
         if (toInterpreterProtoBuffer.has_init())
            processInit(toInterpreterProtoBuffer.init());
         else
            throw shared::exception::CInvalidParameter(
               (boost::format("Unexpected message %1% when initialization") % toInterpreterProtoBuffer.OneOf_case()).str());
         return;
      }

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         if (m_onReceiveHook && m_onReceiveHook(toInterpreterProtoBuffer))
         {
            m_onReceiveHook.clear();
            return;
         }
      }

      switch (toInterpreterProtoBuffer.OneOf_case())
      {
      case interpreter_IPC::toInterpreter::msg::kSystem: processSystem(toInterpreterProtoBuffer.system());
         break;
      case interpreter_IPC::toInterpreter::msg::kAvalaibleRequest: processAvailableRequest(toInterpreterProtoBuffer.avalaiblerequest());
         break;
      case interpreter_IPC::toInterpreter::msg::kLoadScriptContentRequest: processLoadScriptContentRequest(
            toInterpreterProtoBuffer.loadscriptcontentrequest());
         break;
      case interpreter_IPC::toInterpreter::msg::kSaveScriptContentRequest: processSaveScriptContent(
            toInterpreterProtoBuffer.savescriptcontentrequest());
         break;
      case interpreter_IPC::toInterpreter::msg::kStartScript: processStartScript(toInterpreterProtoBuffer.startscript());
         break;
      case interpreter_IPC::toInterpreter::msg::kStopScript: processStopScript(toInterpreterProtoBuffer.stopscript());
         break;
      case interpreter_IPC::toInterpreter::msg::kPurgeScriptLog: processPurgeScriptLog(toInterpreterProtoBuffer.purgescriptlog());
         break;
      default:
         throw shared::exception::CInvalidParameter(
            (boost::format("message : unknown message type %1%") % toInterpreterProtoBuffer.OneOf_case()).str());
      }
   }


   void CApiImplementation::waitInitialized() const
   {
      std::unique_lock<std::mutex> lock(m_initializationConditionMutex);
      if (!m_initialized)
      {
         m_initializationCondition.wait(lock);
      }
   }

   void CApiImplementation::processInit(const interpreter_IPC::toInterpreter::Init& msg)
   {
      m_interpreterInformation = boost::make_shared<CInformation>(
         boost::make_shared<const interpreter_IPC::toInterpreter::Information>(msg.interpreterinformation()));
      m_logFile = boost::make_shared<const boost::filesystem::path>(msg.logfile());
      m_logLevel = boost::make_shared<const std::string>(msg.loglevel());
      setInitialized();
   }

   void CApiImplementation::processSystem(const interpreter_IPC::toInterpreter::System& msg)
   {
      if (msg.type() == interpreter_IPC::toInterpreter::System_EventType_kRequestStop)
      {
         // Request the main thread to stop
         m_stopRequested = true;
         getEventHandler().postEvent(kEventStopRequested);

         // Stop this thread
         throw boost::thread_interrupted();
      }
   }

   void CApiImplementation::processAvailableRequest(const interpreter_IPC::toInterpreter::AvalaibleRequest& msg)
   {
      const boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request =
         boost::make_shared<CAvalaibleRequest>(msg,
                                               [&](bool available)
                                               {
                                                  interpreter_IPC::toYadoms::msg ans;
                                                  auto answer = ans.mutable_avalaibleanswer();
                                                  answer->set_avalaible(available);
                                                  send(ans);
                                               },
                                               [&](const std::string& r)
                                               {
                                                  interpreter_IPC::toYadoms::msg ans;
                                                  ans.set_error(r);
                                                  send(ans);
                                               });

      m_interpreterEventHandler.postEvent(kEventAvalaibleRequest, request);
   }

   void CApiImplementation::processLoadScriptContentRequest(const interpreter_IPC::toInterpreter::LoadScriptContentRequest& msg)
   {
      const boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request =
         boost::make_shared<CLoadScriptContentRequest>(msg,
                                                       [&](const std::string& content)
                                                       {
                                                          interpreter_IPC::toYadoms::msg ans;
                                                          auto answer = ans.mutable_loadscriptcontentanswer();
                                                          answer->set_content(content);
                                                          send(ans);
                                                       },
                                                       [&](const std::string& r)
                                                       {
                                                          interpreter_IPC::toYadoms::msg ans;
                                                          auto answer = ans.mutable_loadscriptcontentanswer();
                                                          answer->set_error(r);
                                                          send(ans);
                                                       });

      m_interpreterEventHandler.postEvent(kEventLoadScriptContentRequest, request);
   }

   void CApiImplementation::processSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContentRequest& msg)
   {
      const boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> command =
         boost::make_shared<CSaveScriptContentRequest>(msg,
                                                       [&]()
                                                       {
                                                          interpreter_IPC::toYadoms::msg ans;
                                                          ans.mutable_savescriptcontentanswer();
                                                          send(ans);
                                                       },
                                                       [&](const std::string& r)
                                                       {
                                                          interpreter_IPC::toYadoms::msg ans;
                                                          ans.set_error(r);
                                                          send(ans);
                                                       });
      m_interpreterEventHandler.postEvent(kEventSaveScriptContent, command);
   }

   void CApiImplementation::processStartScript(const interpreter_IPC::toInterpreter::StartScript& msg)
   {
      const boost::shared_ptr<shared::script::yInterpreterApi::IStartScript> request = boost::make_shared<CStartScript>(msg);

      m_interpreterEventHandler.postEvent(kEventStartScript,
                                          request);
   }

   void CApiImplementation::processStopScript(const interpreter_IPC::toInterpreter::StopScript& msg)
   {
      const boost::shared_ptr<shared::script::yInterpreterApi::IStopScript> request = boost::make_shared<CStopScript>(msg);

      m_interpreterEventHandler.postEvent(kEventStopScript,
                                          request);
   }

   void CApiImplementation::processPurgeScriptLog(const interpreter_IPC::toInterpreter::PurgeScriptLog& msg)
   {
      const boost::shared_ptr<shared::script::yInterpreterApi::IPurgeScriptLog> request = boost::make_shared<CPurgeScriptLog>(msg);

      m_interpreterEventHandler.postEvent(kEventPurgeScriptLog,
                                          request);
   }

   void CApiImplementation::setInitialized()
   {
      if (!!m_interpreterInformation && !!m_logFile && !!m_logLevel)
      {
         std::unique_lock<std::mutex> lock(m_initializationConditionMutex);
         m_initialized = true;
         m_initializationCondition.notify_one();
      }
   }

   boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CApiImplementation::getInformation() const
   {
      if (!m_interpreterInformation)
         throw std::runtime_error("Interpreter information not available");

      return m_interpreterInformation;
   }

   const boost::filesystem::path& CApiImplementation::getLogFile() const
   {
      if (!m_logFile)
         throw std::runtime_error("Interpreter instance log file path not available");

      return *m_logFile;
   }

   const std::string& CApiImplementation::getLogLevel() const
   {
      if (!m_logLevel)
         throw std::runtime_error("Plugin instance log level not available");

      return *m_logLevel;
   }

   shared::event::CEventHandler& CApiImplementation::getEventHandler()
   {
      return m_interpreterEventHandler;
   }
} // namespace interpreter_cpp_api	
