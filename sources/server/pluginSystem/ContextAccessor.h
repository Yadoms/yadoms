#pragma once
#include "IContextAccessor.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/communication/MessageQueueRemover.hpp>

namespace pluginSystem //TODO refactorer pour factoriser avec le CContextAccessor des scripts (y'a moyen)
{
   //--------------------------------------------------------------
   /// \brief	yScriptApi context accessor, based on message queues
   //--------------------------------------------------------------
   class CContextAccessor : public IContextAccessor
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] context yPluginApi context instance
      //--------------------------------------------------------------
      explicit CContextAccessor(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> yPluginApi);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CContextAccessor();

   protected:
      // IContextAccessor Implementation
      std::string id() const override;
      void postStopRequest() override;
      void postPluginInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information) override;
      void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) override;
      // [END] IContextAccessor Implementation

      //--------------------------------------------------------------
      /// \brief	Create a unique context accessor ID (unique on full system)
      //--------------------------------------------------------------
      static std::string createId();

      //--------------------------------------------------------------
      /// \brief	Message queue receive thread
      //--------------------------------------------------------------
      void messageQueueReceiveThreaded();

      //--------------------------------------------------------------
      /// \brief	Send a message to plugin
      /// \param[in] pbMsg The message
      //--------------------------------------------------------------
      void send(const toPlugin::msg& pbMsg);

      //--------------------------------------------------------------
      /// \brief	Send a message to plugin and wait answer
      /// \param[in] pbMsg The message
      /// \param[in] checkExpectedMessageFunction Callback checking that answer is the expected one
      /// \param[in] onReceiveFunction Callback to process the received message
      //--------------------------------------------------------------
      void send(const toPlugin::msg& pbMsg,
                boost::function1<bool, const toYadoms::msg&> checkExpectedMessageFunction,
                boost::function1<void, const toYadoms::msg&> onReceiveFunction);

      //--------------------------------------------------------------
      /// \brief	Process a received message
      /// \param[in] message The message data
      /// \param[in] messageSize The message size
      //--------------------------------------------------------------
      void processMessage(boost::shared_ptr<const unsigned char[]> message, size_t messageSize);

      //--------------------------------------------------------------
      /// \brief	Process messages
      /// \param[in] request Received requests
      /// \param[in] messageQueue Message Queue used for answer
      //--------------------------------------------------------------
      void processSetPluginState(const toYadoms::SetPluginState& msg) const;
      void processGetConfiguration(const toYadoms::ConfigurationRequest& msg);
      //TODO
      //void processGetKeywordId(const pbRequest::GetKeywordId& request, boost::interprocess::message_queue& messageQueue);
      //void processGetRecipientId(const pbRequest::GetRecipientId& request, boost::interprocess::message_queue& messageQueue);
      //void processReadKeyword(const pbRequest::ReadKeyword& request, boost::interprocess::message_queue& messageQueue);
      //void processWaitForNextAcquisition(const pbRequest::WaitForNextAcquisition& request, boost::interprocess::message_queue& messageQueue);
      //void processWaitForNextAcquisitions(const pbRequest::WaitForNextAcquisitions& request, boost::interprocess::message_queue& messageQueue);
      //void processWaitForEvent(const pbRequest::WaitForEvent& request, boost::interprocess::message_queue& messageQueue);
      //void processWriteKeyword(const pbRequest::WriteKeyword& request, boost::interprocess::message_queue& messageQueue);
      //void processSendNotification(const pbRequest::SendNotification& request, boost::interprocess::message_queue& messageQueue);

   private:
      //--------------------------------------------------------------
      /// \brief	Message queue max message size & number
      //--------------------------------------------------------------
      static const size_t m_maxMessages;
      static const size_t m_maxMessageSize;

      //--------------------------------------------------------------
      /// \brief	IYPluginApi context instance
      //--------------------------------------------------------------
      boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> m_pluginApi;

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
      ///\brief               The message queue buffer for sending, localy used but defined here to be allocated only once
      //-----------------------------------------------------
      boost::shared_ptr<unsigned char[]> m_sendBuffer;

      boost::thread m_messageQueueReceiveThread;

      mutable boost::recursive_mutex m_onReceiveHookMutex;
      boost::function1<bool, const toYadoms::msg&> m_onReceiveHook;
   };

} // namespace pluginSystem