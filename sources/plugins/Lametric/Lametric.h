#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "LametricConfiguration.h"
#include "UrlManager.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

struct DeviceInformation
{
   std::string deviceName;
   std::string deviceType;
   std::string deviceModel;
};
//--------------------------------------------------------------
/// \brief	This class is an empty plugin example
/// \note   This plugin do nothing, you just have to :
///         - rename all classes of this plugin
///         - populate all code/functions
//--------------------------------------------------------------
class CLametric : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CLametric();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CLametric() = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   static void declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation* deviceInformation);
   void declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation* deviceInformation) const;
   bool isDeviceActive(boost::shared_ptr<yApi::IYPluginApi>& api);
   void fillDeviceInformation(DeviceInformation* deviceInformation) const;
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CLametricConfiguration m_configuration;

   boost::shared_ptr<CUrlManager> m_lametricManager;

   boost::shared_ptr<yApi::historization::CText> m_text;

   static const std::string DeviceName;
   static const std::string TextKeywordName;
};