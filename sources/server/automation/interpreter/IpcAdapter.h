#pragma once
#include "IIpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <interpreter_IPC/toInterpreter.pb.h>
#include <interpreter_IPC/toYadoms.pb.h>

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
         explicit CIpcAdapter(const std::string& interpreterName);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CIpcAdapter();

      protected:
         // IIpcAdapter Implementation
         std::string id() const override;

         void postStopRequest() override;
         void postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information) override;

         void postAvalaibleRequest(boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request) override;
         void postLoadScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request) override;
         void postSaveScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> request) override;
         void postStartScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStartScriptRequest> request) override;
         void postStopScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStopScriptRequest> request) override;
         // [END] IIpcAdapter Implementation

         //--------------------------------------------------------------
         /// \brief	Create a unique context accessor ID (unique on full system)
         //--------------------------------------------------------------
         static std::string createId();

         //--------------------------------------------------------------
         /// \brief	Message queue receive thread
         //--------------------------------------------------------------
         void messageQueueReceiveThreaded();

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
                   const boost::posix_time::time_duration& timeout = boost::posix_time::seconds(10));

         //--------------------------------------------------------------
         /// \brief	Process a received message
         /// \param[in] message The message data
         /// \param[in] messageSize The message size
         //--------------------------------------------------------------
         void processMessage(boost::shared_ptr<const unsigned char[]> message, size_t messageSize);

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
         ///\brief               The message queue buffer for sending, localy used but defined here to be allocated only once
         //-----------------------------------------------------
         boost::shared_ptr<unsigned char[]> m_sendBuffer;

         boost::thread m_messageQueueReceiveThread;

         mutable boost::recursive_mutex m_onReceiveHookMutex;
         boost::function1<bool, const interpreter_IPC::toYadoms::msg&> m_onReceiveHook;
      };
   }
} // namespace automation::interpreter


