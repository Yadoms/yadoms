#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "Features/IFeature.h"
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
   /// \param[in] wuConfiguration     The plugin configuration
   /// \param[in] request             The configuration received for the device
   /// \param[in] location            The GPS location to be used for each device
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

   //--------------------------------------------------------------
   /// \brief	                     Retreive device
   /// \return                      The device
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> getWeatherConditionsDevice();
   boost::shared_ptr<features::IFeature> getAstronomyDevice();
   boost::shared_ptr<features::IFeature> getForecastDevice();

private:

   //--------------------------------------------------------------
   /// \brief	 Creation du device Weather Conditions
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> m_weatherConditions;

   //--------------------------------------------------------------
   /// \brief	 Creation du device Astronomy
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> m_astronomy;

   //--------------------------------------------------------------
   /// \brief	 Creation du device Forecast
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> m_forecast;
};