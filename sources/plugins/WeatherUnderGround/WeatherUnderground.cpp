#include "stdafx.h"
#include "WeatherUnderground.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "WeatherConditions.h"
#include "Astronomy.h"
#include "ForecastDays.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)


CWeatherUnderground::CWeatherUnderground()
   : m_deviceName("WeatherUnderground")
{
}

CWeatherUnderground::~CWeatherUnderground()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshWeatherConditions = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshAstronomy,
   kEvtTimerRefreshForecast10Days
};

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "CWeatherUnderground is starting..." << std::endl;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Event to be sent immediately for the first value
   api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   // Timer used to read periodically the Weather information
   api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));

   // Event to be sent immediately for the first value
   api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   // Timer used to read periodically the Weather information
   api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));

   // Event to be sent immediately for the first value
   api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   // Timer used to read periodically the Weather information
   api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));

   if (!api->deviceExists(m_deviceName))
   {
      std::string m_URL = "www.wunderground.com/";

      api->declareDevice(m_deviceName, m_URL);
   }

   CWeatherConditions m_WeatherConditionsRequester(api, m_configuration, m_deviceName, "conditions.");
   CAstronomy m_AstronomyRequester(api, m_configuration, m_deviceName, "astronomy.");
   CForecastDays m_Forecast10Days(api, m_configuration, m_deviceName, "forecast.");

   // the main loop
   std::cout << "CWeatherUnderground plugin is running..." << std::endl;

   while (1)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            std::cout << "Stop requested" << std::endl;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case kEvtTimerRefreshWeatherConditions:
         {
            std::cout << "Refresh Weather Conditions" << std::endl;

            if (!m_WeatherConditionsRequester.Request(api))
               m_WeatherConditionsRequester.Parse(api, m_configuration);

            if (!m_WeatherConditionsRequester.IsModuleInFault())
               api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case kEvtTimerRefreshAstronomy:
         {
            std::cout << "Refresh Astronomy Information" << std::endl;

            if (!m_AstronomyRequester.Request(api))
               m_AstronomyRequester.Parse(api, m_configuration);

            if (!m_WeatherConditionsRequester.IsModuleInFault())
               api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case kEvtTimerRefreshForecast10Days:
         {
            std::cout << "Refresh Forecast 10 Days Information" << std::endl;

            if (!m_Forecast10Days.Request(api))
            {
               m_Forecast10Days.SetCityName(m_WeatherConditionsRequester.GetCityName());
               m_Forecast10Days.Parse(api, m_configuration);
            }

            if (!m_WeatherConditionsRequester.IsModuleInFault())
               api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());

            m_WeatherConditionsRequester.OnUpdate(api, m_configuration);

            if (!m_WeatherConditionsRequester.Request(api))
               m_WeatherConditionsRequester.Parse(api, m_configuration);

            m_AstronomyRequester.OnUpdate(api, m_configuration);

            if (!m_AstronomyRequester.Request(api))
               m_AstronomyRequester.Parse(api, m_configuration);

            m_Forecast10Days.OnUpdate(api, m_configuration);

            m_Forecast10Days.SetCityName(m_WeatherConditionsRequester.GetCityName());

            if (!m_Forecast10Days.Request(api))
               m_Forecast10Days.Parse(api, m_configuration);

            if (!m_WeatherConditionsRequester.IsModuleInFault() &&
               !m_Forecast10Days.IsModuleInFault() &&
               !m_AstronomyRequester.IsModuleInFault())
            {
               api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
            break;
         }

      default:
         {
            std::cerr << "Unknown message id" << std::endl;
            break;
         }
      }
   }
}

void CWeatherUnderground::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}

