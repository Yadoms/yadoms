#include "stdafx.h"
#include "WeatherUnderground.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "WeatherConditions.h"
#include "Astronomy.h"
#include "ForecastDays.h"
#include <shared/http/HttpMethods.h>
#include <shared/exception/Exception.hpp>
#include "ErrorAnswerHandler.h"
#include "RequestErrorException.hpp"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)


CWeatherUnderground::CWeatherUnderground()
   : m_deviceName("WeatherUnderground")
{}

CWeatherUnderground::~CWeatherUnderground()
{}

// Event IDs
enum
{
   kEvtTimerRefreshWeatherConditions = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshAstronomy,
   kEvtTimerRefreshForecast10Days,
};

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   int weatherConditionsSendingRetry = 0;
   int astronomySendingRetry = 0;
   int forcast10daysSendingRetry = 0;

   std::cout << "CWeatherUnderground is starting..." << std::endl;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));

   api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));

   api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));

   if (!api->deviceExists(m_deviceName))
   {
      std::string m_URL = "www.wunderground.com/";

      api->declareDevice(m_deviceName, m_URL);
   }

   CWeatherConditions m_WeatherConditionsRequester(api, m_configuration, m_deviceName, "conditions.");
   CAstronomy m_AstronomyRequester(api, m_configuration, m_deviceName, "astronomy.");
   CForecastDays m_Forecast10Days(api, m_configuration, m_deviceName, "forecast.");

   std::cout << "CWeatherUnderground plugin is running..." << std::endl;

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // the main loop
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
            shared::CDataContainer returnData;
            try
            {
               returnData = SendUrlRequest(api, m_WeatherConditionsRequester.getUrl(), kEvtTimerRefreshWeatherConditions, weatherConditionsSendingRetry);
               m_WeatherConditionsRequester.parse(api, m_configuration, returnData);
               std::cout << "Refresh Weather Conditions" << std::endl;
               api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
            catch(CRequestErrorException& )
            {}

            break;
         }
      case kEvtTimerRefreshAstronomy:
         {
            shared::CDataContainer returnData;
            try
            {
               returnData = SendUrlRequest(api, m_AstronomyRequester.getUrl(), kEvtTimerRefreshAstronomy, astronomySendingRetry);
               m_AstronomyRequester.parse(api, m_configuration, returnData);
               std::cout << "Refresh Astronomy Information" << std::endl;
               api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
            catch (CRequestErrorException&)
            {}

            break;
         }
      case kEvtTimerRefreshForecast10Days:
         {
            shared::CDataContainer returnData;
            try
            {
               returnData = SendUrlRequest(api, m_Forecast10Days.getUrl(), kEvtTimerRefreshForecast10Days, forcast10daysSendingRetry);
               m_Forecast10Days.setCityName(m_WeatherConditionsRequester.getCityName()); // TODO : Vérifier lorsque m_WeatherConditionsRequester n'est pas utilisé !!
               m_Forecast10Days.parse(api, m_configuration, returnData);
               std::cout << "Refresh Forecast 10 Days Information" << std::endl;
               api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
            catch (CRequestErrorException&)
            {}
            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            shared::CDataContainer returnData;
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());

            m_WeatherConditionsRequester.onUpdate(api, m_configuration);

            // Send a message to send for manage the corresponding information
            api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            m_AstronomyRequester.onUpdate(api, m_configuration);

            // Send a message to send for manage the corresponding information
            api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            m_Forecast10Days.onUpdate(api, m_configuration);
            m_Forecast10Days.setCityName(m_WeatherConditionsRequester.getCityName());

            // Send a message to send for manage the corresponding information
            api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
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

void CWeatherUnderground::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}

shared::CDataContainer CWeatherUnderground::SendUrlRequest(boost::shared_ptr<yApi::IYPluginApi> api, std::string url, int event, int &nbRetry)
{
   try
   {
      shared::CDataContainer data;
      shared::CHttpMethods m_webServer;

      data = m_webServer.SendGetRequest(url);

      ErrorAnswerHandler Response(api, data);

      // TODO : Cracher une exception différente avec le message d'erreur qui va bien!
      if (Response.ContainError())
         throw shared::exception::CException("Response contain error");

      //All is ok we reinitialize the nbRetry
      nbRetry = 0;

      return data;
   }
   catch (shared::exception::CException& e)
   {
      if (nbRetry < 2)
      {
         std::cout << e.what() << ". Retry in 1 minute." << std::endl;
         api->getEventHandler().createTimer(event, shared::event::CEventTimer::kOneShot, boost::posix_time::minutes(1));
         ++nbRetry;
      }
      else
      {
         std::cout << e.what() << ". Stop retry." << std::endl;
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection"); 
      }

      throw CRequestErrorException();
   }
}