#include "stdafx.h"
#include "WUFactory.h"
#include "Features/WeatherConditions.h"
#include "Features/Astronomy.h"
#include "Features/ForecastDays.h"
#include "WeatherUnderground.h"
#include "Features/Location.h"

CWUFactory::CWUFactory(boost::shared_ptr<yApi::IYPluginApi> api, 
                       IWUConfiguration& wuConfiguration)
{
   initializeLiveStations(api, wuConfiguration);

   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   boost::shared_ptr<const shared::ILocation> location = m_liveStations->getCityLocation();
   std::string cityName = m_liveStations->getCity();

   // Create all devices if present
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
      std::string type;
      std::cout << "Name : " << (*devicesIterator) << std::endl;
      try {
         type = api->getDeviceDetails(*devicesIterator).get<std::string>("type");
      }
      catch(std::exception&)
      {}

      if (type == "weather")
      {
         if (!m_weatherConditions)
         {
            m_weatherConditions = boost::make_shared<CWeatherConditions>(api,
                                                                         wuConfiguration,
                                                                         location,
                                                                         (*devicesIterator),
                                                                         cityName);

            api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            m_weatherTimer = api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
            m_weatherTimer->start();
         }
         else
         {
            m_weatherTimer.reset();
            std::cout << "Could not create device weather : already exist." << std::endl;
         }
      }
      
      if (type == "astronomy")
      {
         if (!m_astronomy)
         {
            m_astronomy = boost::make_shared<CAstronomy>(api,
                                                         wuConfiguration,
                                                         location,
                                                         (*devicesIterator));

            api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            m_astronomyTimer = api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
            m_astronomyTimer->start();
         }
         else
         {
            m_astronomyTimer.reset();
            std::cout << "Could not create device astronomy : already exist." << std::endl;
         }
      }

      if (type == "forecast")
      {
         if (!m_forecast)
         {
            m_forecast = boost::make_shared<CForecastDays>(api,
                                                           wuConfiguration,
                                                           location,
                                                           (*devicesIterator),
                                                           cityName);

            api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            m_forecastTimer = api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
            m_forecastTimer->start();
         }
         else
         {
            m_forecastTimer.reset();
            std::cout << "Could not create device forecast : already exist." << std::endl;
         }
      }
   }

   // Create device if any
   createDevice(api, wuConfiguration);
}

void CWUFactory::createDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                              IWUConfiguration& wuConfiguration)
{
   // get the location and name of the selected station
   initializeLiveStations(api, wuConfiguration);
   std::string cityName = m_liveStations->getCity();

   // If astronomy is enabled
   if (wuConfiguration.isAstronomyEnabled())
   {
      if (!m_astronomy)
      {
         m_astronomy = boost::make_shared<CAstronomy>(api, wuConfiguration, m_liveStations->getCityLocation(), "Astronomy");

         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_astronomyTimer = api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
         m_astronomyTimer->start();
      }
      else
      {
         m_astronomy->onPluginUpdate(api, wuConfiguration);
         std::cout << "Update astronomy module" << std::endl;
      }
   }
   else
   {
      m_astronomyTimer.reset();
      m_astronomy.reset();
   }

   // if conditions are enabled
   if (wuConfiguration.isConditionsIndividualKeywordsEnabled() || wuConfiguration.isLiveConditionsEnabled())
   {
      if (!m_weatherConditions)
      {
         m_weatherConditions = boost::make_shared<CWeatherConditions>(api, wuConfiguration, m_liveStations->getCityLocation(), "Weather", cityName);

         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_weatherTimer = api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
         m_weatherTimer->start();
      }
      else
      {
         m_weatherConditions->onPluginUpdate(api, wuConfiguration);
         std::cout << "Update weather module" << std::endl;
      }
   }
   else
   {
      m_weatherConditions.reset();
      m_weatherTimer.reset();
   }

   // If forecast is enabled
   if (wuConfiguration.isForecast10DaysEnabled())
   {
      if (!m_forecast)
      {
         m_forecast = boost::make_shared<CForecastDays>(api, wuConfiguration, m_liveStations->getCityLocation(), "Forecast", cityName);

         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_forecastTimer = api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
         m_forecastTimer->start();
      }
      else
      {
         m_forecast->onPluginUpdate(api, wuConfiguration);
         std::cout << "Update forecast module" << std::endl;
      }
   }
   else
   {
      m_forecast.reset();
      m_forecastTimer.reset();
   }
}

void CWUFactory::initializeLiveStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                        IWUConfiguration& wuConfiguration)
{
   // If the plugin location is checked, we use the the plugin location otherwise we asked Yadoms system
   if (wuConfiguration.pluginLocationEnabled())
      m_liveStations = boost::make_shared<CLiveStations>(wuConfiguration.getLocation());
   else
      m_liveStations = boost::make_shared<CLiveStations>(api);

   //Get all forecast stations to be displayed into the menu (send the request to the website)
   //TODO : A simplifier
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

/*
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
*/
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