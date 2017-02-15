#pragma once
#include "IIpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <script_IPC/scriptToYadoms.pb.h>
#include <script_IPC/yadomsToScript.pb.h>
#include "YScriptApiImplementation.h"
#include <shared/communication/IMessageCutter.h>

namespace automation
{
   namespace script
   {
      //--------------------------------------------------------------
      /// \brief	Interpreter IPC adapter, based on message queues
      //--------------------------------------------------------------
      class CIpcAdapter : public IIpcAdapter
      {
      public:
         explicit CIpcAdapter(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                              int ruleId);
         virtual ~CIpcAdapter();

      protected:
         // IIpcAdapter Implementation
         std::string id() const override;
         // [END] IIpcAdapter Implementation

         //--------------------------------------------------------------
         /// \brief	Create a unique context accessor ID (unique on full system)
         //--------------------------------------------------------------
         static std::string createId();

         //--------------------------------------------------------------
         /// \brief	Message queue receive thread
         /// \param[in] ruleId The rule ID
         //--------------------------------------------------------------
         void messageQueueReceiveThreaded(int ruleId);

         //--------------------------------------------------------------
         /// \brief	Send a message to script
         /// \param[in] pbMsg The message
         //--------------------------------------------------------------
         void send(const script_IPC::toScript::msg& pbMsg);

         //--------------------------------------------------------------
         /// \brief	Send a message to script and wait answer
         /// \param[in] pbMsg The message
         /// \param[in] checkExpectedMessageFunction Callback checking that answer is the expected one
         /// \param[in] onReceiveFunction Callback to process the received message
         //--------------------------------------------------------------
         void send(const script_IPC::toScript::msg& pbMsg,
                   boost::function1<bool, const script_IPC::toYadoms::msg&> checkExpectedMessageFunction,
                   boost::function1<void, const script_IPC::toYadoms::msg&> onReceiveFunction,
                   const boost::posix_time::time_duration& timeout = boost::posix_time::seconds(10));

         //--------------------------------------------------------------
         /// \brief	Process a received message
         /// \param[in] message The message data
         /// \param[in] messageSize The message size
         //--------------------------------------------------------------
         void processMessage(boost::shared_ptr<const unsigned char[]> message,
                             size_t messageSize);

         //--------------------------------------------------------------
         /// \brief	Process messages
         /// \param[in] request Received requests
         /// \param[in] messageQueue Message Queue used for answer
         //--------------------------------------------------------------
         void processGetKeywordId(const script_IPC::toYadoms::GetKeywordId& request);
         void processGetRecipientId(const script_IPC::toYadoms::GetRecipientId& request);
         void processReadKeyword(const script_IPC::toYadoms::ReadKeyword& request);
         void processWaitForNextAcquisition(const script_IPC::toYadoms::WaitForNextAcquisition& request);
         void processWaitForNextAcquisitions(const script_IPC::toYadoms::WaitForNextAcquisitions& request);
         void processWaitForEvent(const script_IPC::toYadoms::WaitForEvent& request);
         void processWriteKeyword(const script_IPC::toYadoms::WriteKeyword& request);
         void processSendNotification(const script_IPC::toYadoms::SendNotification& request);
         void processGetInfo(const script_IPC::toYadoms::GetInfo& requestueue);
         void processGetKeywordsByCapacity(const script_IPC::toYadoms::GetKeywordsByCapacity& request);
         void processGetKeywordName(const script_IPC::toYadoms::GetKeywordName& request);
         void processGetKeywordDeviceName(const script_IPC::toYadoms::GetKeywordDeviceName& request);

      private:
         //--------------------------------------------------------------
         /// \brief	Message queue max message size & number
         //--------------------------------------------------------------
         static const size_t m_maxMessages;
         static const size_t m_maxMessageSize;

         //--------------------------------------------------------------
         /// \brief	IYScriptApi context instance
         //--------------------------------------------------------------
         boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> m_scriptApi;

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
         boost::function1<bool, const script_IPC::toYadoms::msg&> m_onReceiveHook;
      };
   }
} // namespace automation::script


