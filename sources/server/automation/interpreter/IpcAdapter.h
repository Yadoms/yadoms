#pragma once
#include "IIpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <interpreter_IPC/yadomsToInterpreter.pb.h>
#include <interpreter_IPC/interpreterToYadoms.pb.h>
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>
#include <shared/communication/IMessageCutter.h>

namespace automation
{
   namespace interpreter
   {
      //--------------------------------------------------------------
      /// \brief	Interpreter IPC adapter, based on message queues
      //--------------------------------------------------------------
      class CIpcAdapter : public IIpcAdapter
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] interpreterName The interpreter name (for log)
         //--------------------------------------------------------------
         CIpcAdapter(const std::string& interpreterName,
                     boost::shared_ptr<shared::script::yInterpreterApi::IYInterpreterApi> apiImplementation);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CIpcAdapter();

      protected:
         // IIpcAdapter Implementation
         std::string id() const override;

         void postStopRequest() override;
         void postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information,
                       const boost::filesystem::path& logFile,
                       const std::string& logLevel) override;

         void postAvalaibleRequest(boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request) override;
         void postLoadScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request) override;
         void postSaveScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> request) override;
         void postStartScript(boost::shared_ptr<shared::script::yInterpreterApi::IStartScript> request) override;
         void postStopScript(boost::shared_ptr<shared::script::yInterpreterApi::IStopScript> request) override;
         void postPurgeScriptLog(boost::shared_ptr<shared::script::yInterpreterApi::IPurgeScriptLog> request) override;
         // [END] IIpcAdapter Implementation

         //--------------------------------------------------------------
         /// \brief	Create a unique context accessor ID (unique on full system)
         //--------------------------------------------------------------
         static std::string createId();

         //--------------------------------------------------------------
         /// \brief	Message queue receive thread
         /// \param[in] logName The log name
         //--------------------------------------------------------------
         void messageQueueReceiveThreaded(const std::string& logName);

         //--------------------------------------------------------------
         /// \brief	Send a message to interpreter
         /// \param[in] pbMsg The message
         //--------------------------------------------------------------
         void send(const interpreter_IPC::toInterpreter::msg& pbMsg);

         //--------------------------------------------------------------
         /// \brief	Send a message to interpreter and wait answer
         /// \param[in] pbMsg The message
         /// \param[in] checkExpectedMessageFunction Callback checking that answer is the expected one
         /// \param[in] onReceiveFunction Callback to process the received message
         //--------------------------------------------------------------
         void send(const interpreter_IPC::toInterpreter::msg& pbMsg,
                   boost::function1<bool, const interpreter_IPC::toYadoms::msg&> checkExpectedMessageFunction,
                   boost::function1<void, const interpreter_IPC::toYadoms::msg&> onReceiveFunction,
                   const boost::posix_time::time_duration& timeout = boost::posix_time::seconds(30));

         //--------------------------------------------------------------
         /// \brief	Process a received message
         /// \param[in] message The message data
         /// \param[in] messageSize The message size
         //--------------------------------------------------------------
         void processMessage(boost::shared_ptr<const unsigned char[]> message, size_t messageSize);

         void processNotifiyScriptStopped(const interpreter_IPC::toYadoms::NotifiyScriptStopped& notifiyScriptStopped) const;

      private:
         //--------------------------------------------------------------
         /// \brief	Message queue max message size & number
         //--------------------------------------------------------------
         static const size_t m_maxMessages;
         static const size_t m_maxMessageSize;

         //--------------------------------------------------------------
         /// \brief	Interpreter name
         //--------------------------------------------------------------
         const std::string m_interpreterName;

         //--------------------------------------------------------------
         /// \brief	Api implementation
         //--------------------------------------------------------------
         boost::shared_ptr<shared::script::yInterpreterApi::IYInterpreterApi> m_apiImplementation;

         //--------------------------------------------------------------
         /// \brief	Context accessor ID (unique on full system)
         //--------------------------------------------------------------
         std::string m_id;

         const std::string m_sendMessageQueueId;
         const std::string m_receiveMessageQueueId;
         const shared::communication::CMessageQueueRemover m_sendMessageQueueRemover;
         const shared::communication::CMessageQueueRemover m_receiveMessageQueueRemover;
         boost::interprocess::message_queue m_sendMessageQueue;
         boost::interprocess::message_queue m_receiveMessageQueue;

         //-----------------------------------------------------
         ///\brief               The send mutex. Protect m_sendBuffer and m_sendMessageQueue
         //-----------------------------------------------------
         mutable boost::recursive_mutex m_sendMutex;

         //-----------------------------------------------------
         ///\brief               The message cutter, to manage oversized messages
         //-----------------------------------------------------
         boost::shared_ptr<shared::communication::IMessageCutter> m_messageCutter;

         boost::thread m_messageQueueReceiveThread;

         mutable boost::recursive_mutex m_onReceiveHookMutex;
         boost::function1<bool, const interpreter_IPC::toYadoms::msg&> m_onReceiveHook;
      };
   }
} // namespace automation::interpreter


