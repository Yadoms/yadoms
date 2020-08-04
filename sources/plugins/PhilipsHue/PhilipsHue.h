#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Configuration.h"
#include "HueBridgeDiscovery.h"
#include "HueService.h"
#include "Hue.h"
#include "ILightManager.h"
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is an empty plugin example
/// \note   This plugin do nothing, you just have to :
///         - rename all classes of this plugin
///         - populate all code/functions
//--------------------------------------------------------------
class CPhilipsHue : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPhilipsHue();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPhilipsHue();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   void init();
   void fillHuesInformations();
   void startReadingBridgeButtonState();
private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CConfiguration m_configuration;
   boost::shared_ptr<yApi::IYPluginApi> m_api;

   static const std::string PhilipsHueBridgeName;
   std::vector<HueInformations> m_HuesInformations;
   HueInformations m_HueInformations;
   std::vector<boost::shared_ptr<IHueService>> m_huesService;
   boost::shared_ptr<IHueService> m_hueService;
   std::vector<boost::shared_ptr<CUrlManager>> m_urlsManager;
   boost::shared_ptr<CUrlManager> m_urlManager;
   boost::shared_ptr<IHueBridgeDiscovery> m_hueBridgeDiscovery;
   boost::shared_ptr<ILightManager> m_lightManager;

   std::vector<HueLightInformations> m_detectedLights;

   void closeReadingBridgeButtonState();
   void declareDevice();

   boost::shared_ptr<yApi::historization::CSwitch> m_switch;
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizers;
};
