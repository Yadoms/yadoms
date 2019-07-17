#include "stdafx.h"
#include "Weather.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "OpenWeatherService.h"
#include <shared/Log.h>
#include <Poco/Net/HTTPClientSession.h>

IMPLEMENT_PLUGIN(CWeather)

const boost::posix_time::time_duration CWeather::RequestPeriodicity(boost::posix_time::hours(3));
const std::string CWeather::ForecastWeatherDevicePrefix("Forecast weather day + ");
const int CWeather::NbForecastDays(3);


// Event IDs
enum
{
   kMeasureTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,
};

void CWeather::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CWeather is starting...";

   m_configuration.initializeWith(api->getConfiguration());
   m_configuration.trace();

   m_weatherService = boost::make_shared<COpenWeatherService>(api,
                                                              m_configuration.apiKey());
   declareDevices();

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   requestWeather(api);
   const auto periodicRequestTimer = api->getEventHandler().createTimer(kMeasureTimerEventId,
                                                                        shared::event::CEventTimer::kPeriodic,
                                                                        RequestPeriodicity);

   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            periodicRequestTimer->stop();
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration...";

            m_configuration.initializeWith(newConfiguration);
            m_configuration.trace();

            periodicRequestTimer->stop();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            // Start a measure now and restart timer for next measures
            requestWeather(api);
            periodicRequestTimer->start(RequestPeriodicity);

            break;
         }

      case kMeasureTimerEventId:
         {
            requestWeather(api);
            break;
         }

      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CWeather::declareDevices() const
{
   m_weatherService->declareDevices();


   //for (auto forecastDay = 0; forecastDay < NbForecastDays; ++forecastDay)//TODO revoir ?
   //{
   //   const auto deviceName = ForecastWeatherDevicePrefix + std::to_string(forecastDay);
   //   if (!api->deviceExists(LiveWeatherDeviceName))
   //   {
   //      api->declareDevice(deviceName,
   //                         deviceName,
   //                         deviceName,
   //                         m_forcastPerDayKeywords[forecastDay]);
   //   }
   //}
}

void CWeather::requestWeather(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   YADOMS_LOG(information) << "Request web service...";

   try
   {
      m_weatherService->requestWeather(getLocation(api));
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Error requesting weather data, " << exception.what();
      //TODO ajouter réessais
      //TODO gérer plugin state
   }
}


boost::shared_ptr<const shared::ILocation> CWeather::getLocation(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (m_configuration.useSpecificLocation())
   {
      return m_configuration.specificLocation();   
   }
   else
   {
      return api->getYadomsInformation()->location();
   }
}
