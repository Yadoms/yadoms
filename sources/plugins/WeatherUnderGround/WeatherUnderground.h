#pragma once
#include "WUConfiguration.h"
#include <plugin_cpp_api/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the Weather Underground plugin
/// \note   This plugin send periodically forecast information from a Web Site were The user is indentified
//--------------------------------------------------------------
class CWeatherUnderground : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CWeatherUnderground();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CWeatherUnderground();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData);

private:
   //--------------------------------------------------------------
   /// \brief	The plugin Name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CWUConfiguration m_configuration;
};

