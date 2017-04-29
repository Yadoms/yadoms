#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "IConfiguration.h"
#include "IEngine.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	OneWire plugin
//--------------------------------------------------------------
class COneWire : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   COneWire();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~COneWire();

protected:
   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief Update the configuration of a device
   /// \param [in] devices          1-wire available devices
   /// \param [in] deviceConfiguration  The nex device configuration
   //--------------------------------------------------------------
   void onDeviceConfiguration(std::map<std::string, boost::shared_ptr<device::IDevice> >& devices,
                              boost::shared_ptr<const yApi::ISetDeviceConfiguration> deviceConfiguration);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] devices          1-wire available devices
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(std::map<std::string, boost::shared_ptr<device::IDevice> >& devices,
                  boost::shared_ptr<const yApi::IDeviceCommand> command);

   //--------------------------------------------------------------
   /// \brief Update our 1-wire network image (if new devices were recently connected) and create devices in Yadoms database
   /// \param[in] api                   pointer to the API
   /// \param[in] devices               Current network image to update
   /// \param[in] foundDevices          The network new scan result
   //--------------------------------------------------------------
   void updateNetwork(boost::shared_ptr<yApi::IYPluginApi> api,
                      std::map<std::string, boost::shared_ptr<device::IDevice> >& devices,
                      const std::map<std::string, boost::shared_ptr<device::IDevice> >& foundDevices);

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<IConfiguration> m_configuration;

   //--------------------------------------------------------------
   /// \brief	The 1-wire engine
   //--------------------------------------------------------------
   boost::shared_ptr<IEngine> m_engine;
};
