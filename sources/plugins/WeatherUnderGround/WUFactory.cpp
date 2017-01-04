#include "stdafx.h"
#include "WUFactory.h"
#include "Features/WeatherConditions.h"
#include "Features/Astronomy.h"
#include "Features/ForecastDays.h"
#include "WeatherUnderground.h"
#include "Features/Location.h"

CWUFactory::CWUFactory(boost::shared_ptr<yApi::IYPluginApi> api, 
                       IWUConfiguration& wuConfiguration):
   m_developerMode(api->getYadomsInformation()->developperMode())
{
   initializeLiveStations(api, wuConfiguration);

   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   boost::shared_ptr<const shared::ILocation> location = m_lookupInformation->getCityLocation();

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
         m_weatherConditions = createorUpdateWeatherDevice(api, wuConfiguration);
      }
      
      if (type == "astronomy")
      {
         m_astronomy = createorUpdateAstronomyDevice(api, wuConfiguration);
      }

      if (type == "forecast")
      {
         m_forecast = createorUpdateForecastDevice(api, wuConfiguration);
      }
   }
}

void CWUFactory::createDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                              IWUConfiguration& wuConfiguration)
{
   // get the location and name of the selected station
   initializeLiveStations(api, wuConfiguration);

   // If astronomy is enabled
   m_astronomy = createorUpdateAstronomyDevice(api, wuConfiguration);

   // if conditions are enabled
   m_weatherConditions = createorUpdateWeatherDevice(api, wuConfiguration);

   // If forecast is enabled
   m_forecast = createorUpdateForecastDevice(api, wuConfiguration);
}

void CWUFactory::initializeLiveStations(boost::shared_ptr<yApi::IYPluginApi> api,
                                        IWUConfiguration& wuConfiguration)
{
   // If the plugin location is checked, we use the the plugin location otherwise we asked Yadoms system
   if (wuConfiguration.pluginLocationEnabled())
      m_lookupInformation = boost::make_shared<CLiveStations>(wuConfiguration.getLocation());
   else
      m_lookupInformation = boost::make_shared<CLiveStations>(api);

   if (m_developerMode)
   {
      std::cout << "longitude :" << m_lookupInformation->getCityLocation()->longitude() << std::endl;
      std::cout << "latitude :" << m_lookupInformation->getCityLocation()->latitude() << std::endl;
   }

   //Get location information
   m_lookupInformation->processLookUp(api, wuConfiguration.getAPIKey());
}

boost::shared_ptr<features::IFeature> CWUFactory::createorUpdateWeatherDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                              IWUConfiguration& wuConfiguration)
{
   if (!m_weatherConditions)
   {
      if (wuConfiguration.isLiveConditionsEnabled())
      {
         m_weatherConditions = boost::make_shared<CWeatherConditions>(api, wuConfiguration, m_lookupInformation->getCityLocation(), "Weather", m_lookupInformation->getCity());

         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_weatherTimer = api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
         m_weatherTimer->start();
      }
   }
   else
   {
      if (wuConfiguration.isLiveConditionsEnabled())
      {
         m_weatherConditions->onPluginUpdate(api, wuConfiguration);
         std::cout << "Update weather device" << std::endl;
      }
      else
      {
         std::cout << "remove weather device" << std::endl;
         api->removeDevice(m_weatherConditions->getName());
         m_weatherConditions.reset();
         m_weatherTimer->stop();
         m_weatherTimer.reset();
      }
   }

   return m_weatherConditions;
}

boost::shared_ptr<features::IFeature> CWUFactory::createorUpdateAstronomyDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                IWUConfiguration& wuConfiguration)
{
   if (!m_astronomy)
   {
      if (wuConfiguration.isAstronomyEnabled())
      {
         m_astronomy = boost::make_shared<CAstronomy>(api, wuConfiguration, m_lookupInformation->getCityLocation(), "Astronomy");

         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_astronomyTimer = api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
         m_astronomyTimer->start();
      }
   }
   else
   {
      if (wuConfiguration.isAstronomyEnabled())
      {
         m_astronomy->onPluginUpdate(api, wuConfiguration);
         std::cout << "Update astronomy module" << std::endl;
      }
      else
      {
         std::cout << "remove astronomy device" << std::endl;
         api->removeDevice(m_astronomy->getName());
         m_astronomy.reset();
         m_astronomyTimer->stop();
         m_astronomyTimer.reset();
      }
   }

   return m_astronomy;
}

boost::shared_ptr<features::IFeature> CWUFactory::createorUpdateForecastDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                               IWUConfiguration& wuConfiguration)
{
   if (!m_forecast)
   {
      if (wuConfiguration.isForecast10DaysEnabled())
      {
         m_forecast = boost::make_shared<CForecastDays>(api, wuConfiguration, m_lookupInformation->getCityLocation(), "Forecast", m_lookupInformation->getCity());
         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         m_forecastTimer = api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
         m_forecastTimer->start();
      }
   }
   else
   {
      if (wuConfiguration.isForecast10DaysEnabled())
      {
         m_forecast->onPluginUpdate(api, wuConfiguration);
         std::cout << "Update forecast module" << std::endl;
      }
      else
      {
         std::cout << "remove forecast device" << std::endl;
         api->removeDevice(m_forecast->getName());
		 m_forecastTimer->stop();
         m_forecastTimer.reset();
         m_forecast.reset();
      }
   }

   return m_forecast;
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
{
   std::cout << "Destruction Factory" << std::endl;
}