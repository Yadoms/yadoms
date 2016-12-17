#include "stdafx.h"
#include "WUFactory.h"
#include "Features/WeatherConditions.h"
#include "Features/Astronomy.h"
#include "Features/ForecastDays.h"
#include "deviceConfiguration.h"
#include "WeatherUnderground.h"
#include "Features/Location.h"

CWUFactory::CWUFactory(boost::shared_ptr<yApi::IYPluginApi> api, 
                       IWUConfiguration& wuConfiguration)
{
   initializeLiveStations(api, wuConfiguration);

   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   // Create all devices if present
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
	   std::string type = "";
      boost::shared_ptr<const shared::ILocation> location;
      std::string stationName = "";

      // plugin state have no type
	   try {
         //retrieve the type
		   type = api->getDeviceDetails(*devicesIterator).get<std::string>("type");

         //retrieve location information
         location = boost::make_shared<location::CLocation>(
            api->getDeviceDetails(*devicesIterator).get<double>("long"),
            api->getDeviceDetails(*devicesIterator).get<double>("lat"),
            0);

         //retrieve the station name
         stationName = api->getDeviceDetails(*devicesIterator).get<std::string>("stationName");
	   }
	   catch (...)
	   {}

      std::cout << "Name : " << (*devicesIterator) << std::endl;
      std::cout << "type : " << type << std::endl;

      if (type == "WeatherConditions")
      {
         m_weatherConditions = boost::make_shared<CWeatherConditions>(api, 
                                                                      wuConfiguration, 
                                                                      boost::make_shared<CdeviceConfiguration>(api->getDeviceConfiguration(*devicesIterator)),
                                                                      location,
                                                                      (*devicesIterator),
                                                                      stationName);
      }
      else if (type == "Astronomy")
      {
         m_astronomy = boost::make_shared<CAstronomy>(api, 
                                                      wuConfiguration, 
                                                      boost::make_shared<CdeviceConfiguration>(api->getDeviceConfiguration(*devicesIterator)),
                                                      location,
                                                      (*devicesIterator));
      }
      else if (type == "Forecast")
      {
         m_forecast = boost::make_shared<CForecastDays>(api, 
                                                    wuConfiguration, 
                                                    boost::make_shared<CdeviceConfiguration>(api->getDeviceConfiguration(*devicesIterator)),
                                                    location,
                                                    (*devicesIterator),
                                                    stationName);
      }  
   }
}

std::string CWUFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                             IWUConfiguration& wuConfiguration,
                                             boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> request)
{
   boost::shared_ptr<features::IFeature> returnModule;
   boost::shared_ptr<IdeviceConfiguration> deviceConfiguration = boost::make_shared<CdeviceConfiguration>(request->getData().getConfiguration());

   auto selection = request->getData().getConfiguration().get<int>("LiveStations");

   // get the location and name of the selected station
   boost::shared_ptr<const shared::ILocation> location = m_liveStations->getStationLocation(selection);
   std::string stationName = m_liveStations->getStationName(selection);

   // If astronomy is enabled
   if (deviceConfiguration->isAstronomyEnabled())
   {
      if (!m_astronomy)
      {
         m_astronomy = boost::make_shared<CAstronomy>(api, wuConfiguration, deviceConfiguration, location, request->getData().getDeviceName());
         request->sendSuccess(m_astronomy->getName());
         returnModule = m_astronomy;

         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_astronomyTimer = api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
         m_astronomyTimer->start();
      }
      else
      {
         m_astronomyTimer.reset();;
         std::cout << "Could not create device " << request->getData().getDeviceName() << " : already exist." << std::endl;
         request->sendError("device already exist !");
      }
   }

   // if conditions are enabled
   if (deviceConfiguration->isConditionsIndividualKeywordsEnabled() || deviceConfiguration->isLiveConditionsEnabled())
   {
      if (!m_weatherConditions)
      {
         m_weatherConditions = boost::make_shared<CWeatherConditions>(api, wuConfiguration, deviceConfiguration, location, request->getData().getDeviceName(), stationName);
         request->sendSuccess(m_weatherConditions->getName());
         returnModule = m_weatherConditions;

         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_weatherTimer = api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
         m_weatherTimer->start();
      }
      else
      {
         m_weatherTimer.reset();
         std::cout << "Could not create device " << request->getData().getDeviceName() << " : already exist." << std::endl;
         request->sendError("device already exist !");
      }
   }

   // If forecast is enabled
   if (deviceConfiguration->isForecast10DaysEnabled())
   {
      if (!m_forecast)
      {
         m_forecast = boost::make_shared<CForecastDays>(api, wuConfiguration, deviceConfiguration, location, request->getData().getDeviceName(), stationName);
         request->sendSuccess(m_forecast->getName()); 
         returnModule = m_forecast;

         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_forecastTimer = api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
         m_forecastTimer->start();
      }
      else
      {
         m_forecastTimer.reset();
         std::cout << "Could not create device " << request->getData().getDeviceName() << " : already exist." << std::endl;
         request->sendError("device already exist !");
      }
   }

   return returnModule->getName();
}

void CWUFactory::initializeLiveStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                        IWUConfiguration& wuConfiguration)
{
   if (m_liveStations)
      m_liveStations.reset();

   // If the plugin location is checked, we use the the plugin location otherwise we asked Yadoms system
   if (wuConfiguration.pluginLocationEnabled())
      m_liveStations = boost::make_shared<CLiveStations>(wuConfiguration.getLocation());
   else
      m_liveStations = boost::make_shared<CLiveStations>(api);

   //Get all forecast stations to be displayed into the menu (send the request to the website)
   m_liveStations->getAllStations(api, wuConfiguration.getAPIKey());
}

void CWUFactory::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved)
{
   if (m_weatherConditions->getName() == deviceRemoved)
   {
      m_weatherConditions.reset();
      m_weatherTimer.reset();
   }
   else if (m_astronomy->getName() == deviceRemoved)
   {
      m_astronomy.reset();
      m_astronomyTimer.reset();
   }
   else if (m_forecast->getName() == deviceRemoved)
   {
      m_forecast.reset();
      m_forecastTimer.reset();
   }
   else
   {
      std::cerr << "device " << deviceRemoved << " not found" << std::endl;
   }
}

void CWUFactory::onDeviceSetConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, 
                                          const std::string& deviceName, 
                                          IWUConfiguration& wuConfiguration, 
                                          const shared::CDataContainer newConfiguration)
{
   boost::shared_ptr<IdeviceConfiguration> deviceConfiguration = boost::make_shared<CdeviceConfiguration>(newConfiguration);

   auto selection = newConfiguration.get<int>("LiveStations");

   // get the location and name of the selected station
   boost::shared_ptr<const shared::ILocation> location = m_liveStations->getStationLocation(selection);
   std::string stationName = m_liveStations->getStationName(selection);

   if (m_weatherConditions->getName() == deviceName)
   {
      m_weatherConditions->onDeviceUpdate(api, wuConfiguration, deviceConfiguration, location);
      m_weatherConditions->setCityName(stationName);
   }
   else if (m_astronomy->getName() == deviceName)
   {
      m_astronomy->onDeviceUpdate(api, wuConfiguration, deviceConfiguration, location);
   }
   else if (m_forecast->getName() == deviceName)
   {
      m_forecast->onDeviceUpdate(api, wuConfiguration, deviceConfiguration, location);
      m_forecast->setCityName(stationName);
   }
   else
   {
      std::cerr << "device " << deviceName << " not found" << std::endl;
   }
}

shared::CDataContainer CWUFactory::bindAvailableStations()
{
   return m_liveStations->bindAvailableStations();
}

boost::shared_ptr<features::IFeature> CWUFactory::getWeatherConditionsDevice()
{
   return m_weatherConditions;
}

boost::shared_ptr<features::IFeature> CWUFactory::getAstronomyDevice()
{
   return m_astronomy;
}

boost::shared_ptr<features::IFeature> CWUFactory::getForecastDevice()
{
   return m_forecast;
}

CWUFactory::~CWUFactory()
{}