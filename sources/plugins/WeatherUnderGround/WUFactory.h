#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "Features/IFeature.h"
#include "IWUConfiguration.h"
#include "Features/LiveStations.h"

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
   void createDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                     IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    initialize forecast stations
   /// \param[in] api                  yPluginApi API
   /// \param[in] wuConfiguration     The plugin configuration
   //--------------------------------------------------------------
   void initializeLiveStations(boost::shared_ptr<yApi::IYPluginApi> api,
                               IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	                     Retreive device
   /// \return                      The device
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> getWeatherConditionsDevice() const;
   boost::shared_ptr<features::IFeature> getAstronomyDevice() const;
   boost::shared_ptr<features::IFeature> getForecastDevice() const;

private:

   //--------------------------------------------------------------
   /// \brief	    create or update Weather device
   /// \param[in] api                  yPluginApi API
   /// \param[in] wuConfiguration     The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> createorUpdateWeatherDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                     IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    create or update Astronomy device
   /// \param[in] api                  yPluginApi API
   /// \param[in] wuConfiguration     The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> createorUpdateAstronomyDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                       IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    create or update Forecast device
   /// \param[in] api                  yPluginApi API
   /// \param[in] wuConfiguration     The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<features::IFeature> createorUpdateForecastDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                      IWUConfiguration& wuConfiguration);

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

   //--------------------------------------------------------------
   /// \brief	Periodic timer for t he weather refresh
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_weatherTimer;

   //--------------------------------------------------------------
   /// \brief	Periodic timer for t he astronomy refresh
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_astronomyTimer;

   //--------------------------------------------------------------
   /// \brief	Periodic timer for t he forecast refresh
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_forecastTimer;

   //--------------------------------------------------------------
   /// \brief
   //--------------------------------------------------------------
   boost::shared_ptr<CLiveStations> m_lookupInformation;
};