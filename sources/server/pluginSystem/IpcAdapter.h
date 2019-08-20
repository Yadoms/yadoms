#pragma once
#include "IIpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <plugin_IPC/pluginToYadoms.pb.h>
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include "yPluginApiImplementation.h"
#include <shared/communication/IMessageCutter.h>

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
      /// \param[in] yPluginApi yPluginApi context instance
      //--------------------------------------------------------------
      explicit CIpcAdapter(boost::shared_ptr<CYPluginApiImplementation> yPluginApi);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CIpcAdapter();

   protected:
      // IIpcAdapter Implementation
      std::string id() const override;
      void postStopRequest() override;
      void postInit(boost::shared_ptr<const shared::plugin::information::IInformation> information,
                    const boost::filesystem::path& dataPath,
                    const boost::filesystem::path& logFile,
                    const std::string& logLevel,
                    Poco::Nullable<std::string> proxyHost,
                    Poco::Nullable<Poco::UInt16> proxyPort,
                    Poco::Nullable<std::string> proxyUsername,
                    Poco::Nullable<std::string> proxyPassword,
                    Poco::Nullable<std::string> proxyBypass) override;
      void postUpdateConfiguration(const shared::CDataContainer& newConfiguration) override;
      void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) override;
      void postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request) override;
      void postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration>& command) override;
      void postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand) override;
      void postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery, const std::string& taskId) override;
      void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) override;
      void postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event) override;
      // [END] IIpcAdapter Implementation

      //--------------------------------------------------------------
      /// \brief	Create a unique context accessor ID (unique on full system)
      //--------------------------------------------------------------
      static std::string createId();

      //--------------------------------------------------------------
      /// \brief	Message queue receive thread
      //--------------------------------------------------------------
      void messageQueueReceiveThreaded(int pluginId);

      //--------------------------------------------------------------
      /// \brief	Send a message to plugin
      /// \param[in] pbMsg The message
      //--------------------------------------------------------------
      void send(const plugin_IPC::toPlugin::msg& pbMsg);

      //--------------------------------------------------------------
      /// \brief	Send a message to plugin and wait answer
      /// \param[in] pbMsg The message
      /// \param[in] checkExpectedMessageFunction Callback checking that answer is the expected one
      /// \param[in] onReceiveFunction Callback to process the received message
      //--------------------------------------------------------------
      void send(const plugin_IPC::toPlugin::msg& pbMsg,
                boost::function1<bool, const plugin_IPC::toYadoms::msg&> checkExpectedMessageFunction,
                boost::function1<void, const plugin_IPC::toYadoms::msg&> onReceiveFunction,
                const boost::posix_time::time_duration& timeout = boost::posix_time::seconds(10));

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
      void processSetPluginState(const plugin_IPC::toYadoms::SetPluginState& msg) const;
      void processGetConfiguration(const plugin_IPC::toYadoms::ConfigurationRequest& msg);
      void processDeviceExistsRequest(const plugin_IPC::toYadoms::DeviceExitsRequest& msg);
      void processDeviceDetailsRequest(const plugin_IPC::toYadoms::DeviceDetailsRequest& msg);
      void processUpdateDeviceDetails(const plugin_IPC::toYadoms::UpdateDeviceDetails& msg) const;
      void processAllDevicesRequest(const plugin_IPC::toYadoms::AllDevicesRequest& msg);
      void processKeywordExistsRequest(const plugin_IPC::toYadoms::KeywordExitsRequest& msg);
      void processDeclareDevice(const plugin_IPC::toYadoms::DeclareDevice& msg) const;
      void processDeclareKeyword(const plugin_IPC::toYadoms::DeclareKeyword& msg) const;
      void processRecipientValueRequest(const plugin_IPC::toYadoms::RecipientValueRequest& msg);
      void processFindRecipientsFromFieldRequest(const plugin_IPC::toYadoms::FindRecipientsFromFieldRequest& msg);
      void processRecipientFieldExitsRequest(const plugin_IPC::toYadoms::RecipientFieldExitsRequest& msg);
      void processHistorizeData(const plugin_IPC::toYadoms::HistorizeData& msg) const;
      void processYadomsInformationRequest(const plugin_IPC::toYadoms::YadomsInformationRequest& msg);
      void processRemoveDeviceRequest(const plugin_IPC::toYadoms::RemoveDevice& msg) const;
      void processRemoveKeywordRequest(const plugin_IPC::toYadoms::RemoveKeyword& msg) const;
      void processAllKeywordsRequest(const plugin_IPC::toYadoms::AllKeywordsRequest& msg);
      void processDeclareKeywords(const plugin_IPC::toYadoms::DeclareKeywords& msg) const;
      void processDeviceModelRequest(const plugin_IPC::toYadoms::DeviceModelRequest& msg);
      void processUpdateDeviceModel(const plugin_IPC::toYadoms::UpdateDeviceModel& msg) const;
      void processDeviceTypeRequest(const plugin_IPC::toYadoms::DeviceTypeRequest& msg);
      void processUpdateDeviceType(const plugin_IPC::toYadoms::UpdateDeviceType& msg) const;
      void processDeviceConfigurationRequest(const plugin_IPC::toYadoms::DeviceConfigurationRequest& msg);
      void processUpdateDeviceConfiguration(const plugin_IPC::toYadoms::UpdateDeviceConfiguration& msg) const;
      void processExtraQueryProgression(const plugin_IPC::toYadoms::ExtraQueryProgression& msg) const;
      void processSetDeviceState(const plugin_IPC::toYadoms::SetDeviceState& msg) const;

   private:
      //--------------------------------------------------------------
      /// \brief	Message queue max message size & number
      //--------------------------------------------------------------
      static const size_t m_maxMessages;
      static const size_t m_maxMessageSize;

      //--------------------------------------------------------------
      /// \brief	IYPluginApi context instance
      //--------------------------------------------------------------
      boost::shared_ptr<CYPluginApiImplementation> m_pluginApi;

      //--------------------------------------------------------------
      /// \brief	Context accessor ID (unique on full system)
      //--------------------------------------------------------------
      std::string m_id;

      //--------------------------------------------------------------
      /// \brief	SEND message queue ID
      //--------------------------------------------------------------
      const std::string m_sendMessageQueueId;

      //--------------------------------------------------------------
      /// \brief	RECEIVE message queue ID
      //--------------------------------------------------------------
      const std::string m_receiveMessageQueueId;

      //--------------------------------------------------------------
      /// \brief	SEND message queue remover
      //--------------------------------------------------------------
      const shared::communication::CMessageQueueRemover m_sendMessageQueueRemover;

      //--------------------------------------------------------------
      /// \brief	RECEIVE message queue remover
      //--------------------------------------------------------------
      const shared::communication::CMessageQueueRemover m_receiveMessageQueueRemover;

      //--------------------------------------------------------------
      /// \brief	SEND message queue ID
      //--------------------------------------------------------------
      boost::interprocess::message_queue m_sendMessageQueue;

      //--------------------------------------------------------------
      /// \brief	RECEIVE message queue ID
      //--------------------------------------------------------------
      boost::interprocess::message_queue m_receiveMessageQueue;

      //-----------------------------------------------------
      ///\brief               The send mutex. Protect m_sendBuffer and m_sendMessageQueue
      //-----------------------------------------------------
      mutable boost::recursive_mutex m_sendMutex;

      //-----------------------------------------------------
      ///\brief               The message cutter, to manage oversized messages
      //-----------------------------------------------------
      boost::shared_ptr<shared::communication::IMessageCutter> m_messageCutter;

      //-----------------------------------------------------
      ///\brief               The receiving thread
      //-----------------------------------------------------
      boost::thread m_messageQueueReceiveThread;

      //-----------------------------------------------------
      ///\brief               The receiving hook mutex
      //-----------------------------------------------------
      mutable boost::recursive_mutex m_onReceiveHookMutex;

      //-----------------------------------------------------
      ///\brief               The receiving hook event
      //-----------------------------------------------------
      boost::function1<bool, const plugin_IPC::toYadoms::msg&> m_onReceiveHook;

      //-----------------------------------------------------
      ///\brief               The pending extra queries
      //-----------------------------------------------------
      std::map<std::string, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery>> m_pendingExtraQueries;
   };
} // namespace pluginSystem
