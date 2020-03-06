#pragma once
#include <shared/plugin/information/IInformation.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include <Poco/Nullable.h>


namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	yScriptApi IPC adapter interface, used by plugin to dial with Yadoms
   //--------------------------------------------------------------
   class IIpcAdapter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IIpcAdapter()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Get the context accessor ID (unique on full system)
      /// \return The context accessor ID
      //--------------------------------------------------------------
      virtual std::string id() const = 0;

      //--------------------------------------------------------------
      /// \brief	Post the plugin stop request
      //--------------------------------------------------------------
      virtual void postStopRequest() = 0;

      //--------------------------------------------------------------
      /// \brief	Post the plugin information
      /// \param[in] information The plugin information
      /// \param[in] dataPath    The plugin instance data path
      /// \param[in] logFile     The plugin instance log file path
      /// \param[in] logLevel    The log level to use
      /// \param[in] proxyHost   The proxy host (empty if no proxy)
      /// \param[in] proxyPort   The proxy port
      /// \param[in] proxyUsername  The proxy username
      /// \param[in] proxyPassword  The proxy password
      /// \param[in] proxyBypass    The proxy bypass regex
      //--------------------------------------------------------------
      virtual void postInit(boost::shared_ptr<const shared::plugin::information::IInformation> information,
                            const boost::filesystem::path& dataPath,
                            const boost::filesystem::path& logFile,
                            const std::string& logLevel,
                            Poco::Nullable<std::string> proxyHost,
                            Poco::Nullable<Poco::UInt16> proxyPort,
                            Poco::Nullable<std::string> proxyUsername,
                            Poco::Nullable<std::string> proxyPassword,
                            Poco::Nullable<std::string> proxyBypass) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a configuration update to plugin instance
      /// \param [in] newConfiguration The new configuration
      //--------------------------------------------------------------
      virtual void postUpdateConfiguration(const shared::CDataContainerSharedPtr& newConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a custom query request to a plugin
      /// \param [in] request    Request
      //--------------------------------------------------------------
      virtual void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a device configuration schema request to a plugin
      /// \param [in] request    Request
      //--------------------------------------------------------------
      virtual void postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief                 Set a new device configuration to a plugin
      /// \param [in] command    Set device configuration command
      //--------------------------------------------------------------
      virtual void postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration>& command) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a device command to a plugin
      /// \param [in] deviceCommand The command
      //--------------------------------------------------------------
      virtual void postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post an extra command to a plugin
      /// \param [in] extraQuery The command
      //--------------------------------------------------------------
      virtual void postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery, const std::string& taskId) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a manually device creation request to a plugin
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      virtual void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a device removed notification to a plugin
      /// \param [in] event      The notification
      //--------------------------------------------------------------
      virtual void postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event) = 0;
   };
} // namespace pluginSystem
