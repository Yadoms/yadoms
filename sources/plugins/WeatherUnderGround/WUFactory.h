#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "Modules/IModule.h"
#include "IWUConfiguration.h"
#include "IdeviceConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CWUFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] api                 yPluginApi API
   /// \param[in] wuConfiguration     The plugin configuration
   //--------------------------------------------------------------
   CWUFactory(boost::shared_ptr<yApi::IYPluginApi> api,
              IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWUFactory();

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                    IWUConfiguration& wuConfiguration,
                                    boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> request,
                                    boost::shared_ptr<const shared::ILocation> location);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] receivedValue    The received value from the interrupt
   //--------------------------------------------------------------
   void removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved);

   boost::shared_ptr<modules::IModule> getWeatherConditionsDevice();
   boost::shared_ptr<modules::IModule> getAstronomyDevice();
   boost::shared_ptr<modules::IModule> getForecastDevice();

private:

   //--------------------------------------------------------------
   /// \brief	 Creation du device Weather Conditions
   //--------------------------------------------------------------
   boost::shared_ptr<modules::IModule> m_weatherConditions;

   //--------------------------------------------------------------
   /// \brief	 Creation du device Astronomy
   //--------------------------------------------------------------
   boost::shared_ptr<modules::IModule> m_astronomy;

   //--------------------------------------------------------------
   /// \brief	 Creation du device Forecast
   //--------------------------------------------------------------
   boost::shared_ptr<modules::IModule> m_forecast;
};