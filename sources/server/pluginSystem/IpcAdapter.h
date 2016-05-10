#pragma once
#include "IIpcAdapter.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/communication/MessageQueueRemover.hpp>
#include <plugin_IPC/plugin_IPC.h>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	yPluginApi IPC adapter, based on message queues
   //--------------------------------------------------------------
   class CIpcAdapter : public IIpcAdapter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] context yPluginApi context instance
      //--------------------------------------------------------------
      explicit CIpcAdapter(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> yPluginApi);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CIpcAdapter();

   protected:
      // IIpcAdapter Implementation
      std::string id() const override;
      void postStopRequest() override;
      void postPluginInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information) override;
      void postUpdateConfiguration(const shared::CDataContainer& newConfiguration) override;
      void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) override;
      void postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand) override;
      void postExtraCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand) override;
      void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) override;
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
      void processDeviceExistsRequest(const toYadoms::DeviceExitsRequest& msg);
      void processDeviceDetailsRequest(const toYadoms::DeviceDetailsRequest& msg);
      void processKeywordExistsRequest(const toYadoms::KeywordExitsRequest& msg);
      void processDeclareDevice(const toYadoms::DeclareDevice& msg) const;
      void processDeclareKeyword(const toYadoms::DeclareKeyword& msg) const;
      void processRecipientValueRequest(const toYadoms::RecipientValueRequest& msg);
      void processFindRecipientsFromFieldRequest(const toYadoms::FindRecipientsFromFieldRequest& msg);
      void processRecipientFieldExitsRequest(const toYadoms::RecipientFieldExitsRequest& msg);
      void processHistorizeData(const toYadoms::HistorizeData& msg) const;

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