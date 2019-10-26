#include "stdafx.h"
#include "Weather.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "OpenWeatherService.h"
#include <shared/Log.h>
#include <Poco/Net/HTTPClientSession.h>

IMPLEMENT_PLUGIN(CWeather)

const boost::posix_time::time_duration CWeather::RequestPeriodicity(boost::posix_time::hours(3));


// Event IDs
enum
{
   kMeasureTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,
};

CWeather::CWeather()
   : m_requestTries(0)
{
}

void CWeather::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CWeather is starting...";

   m_configuration.initializeWith(api->getConfiguration());
   m_configuration.trace();

   m_weatherService = boost::make_shared<COpenWeatherService>(api,
                                                              m_configuration.apiKey());
   declareDevices();

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   m_requestTries = 0;
   static const auto Immediately = boost::posix_time::seconds(0);
   m_requestTimer = api->getEventHandler().createTimer(kMeasureTimerEventId,
                                                       shared::event::CEventTimer::kOneShot,
                                                       Immediately);

   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            m_requestTimer->stop();
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

            m_requestTimer->stop();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            // Start a measure now and restart timer for next measures
            requestWeather(api);

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
}

void CWeather::requestWeather(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Request web service...";

   try
   {
      m_weatherService->requestWeather(getLocation(api));

      api->setPluginState(yApi::historization::EPluginState::kRunning);
      m_requestTries = 0;
      m_requestTimer->start(RequestPeriodicity);
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Error requesting weather data, " << exception.what();

      api->setPluginState(yApi::historization::EPluginState::kCustom,
                          "lastRequestFailed",
                          {{"error", exception.what()}});

      if (++m_requestTries >= 3)
      {
         api->setPluginState(yApi::historization::EPluginState::kCustom,
                             "maxTriesAchieved");
         m_requestTries = 0;
         m_requestTimer->start(RequestPeriodicity);
         return;
      }
      m_requestTimer->start(m_weatherService->serviceRecommendedRetryDelay());
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
