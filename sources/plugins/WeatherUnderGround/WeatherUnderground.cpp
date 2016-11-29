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
   : m_deviceName("WeatherUnderground"),
     m_runningState(false)
{}

CWeatherUnderground::~CWeatherUnderground()
{}

// Event IDs
enum
{
   kEvtTimerRefreshWeatherConditions = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshAstronomy,
   kEvtTimerRefreshForecast10Days,
   kEvtPluginState
};

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "CWeatherUnderground is starting..." << std::endl;

   int weatherConditionsSendingRetry = 0;
   int astronomySendingRetry = 0;
   int forecast10daysSendingRetry = 0;

   bool weatherConditionsFinished = false;
   bool astronomyFinished = false;
   bool forecast10daysFinished = false;

   boost::shared_ptr<CWeatherConditions> m_WeatherConditionsRequester;
   boost::shared_ptr<CAstronomy> m_AstronomyRequester;
   boost::shared_ptr<CForecastDays> m_Forecast10Days;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   //shared::CDataContainer returnMessage = SendUrlRequest(api, "http://api.wunderground.com/api" + m_configuration.getAPIKey() + "/geolookup/q/37.7762,89,-122.395234.json", 3);

   try
   {
      m_WeatherConditionsRequester = boost::make_shared<CWeatherConditions>(api, m_configuration);
      m_AstronomyRequester         = boost::make_shared<CAstronomy>(api, m_configuration);
      m_Forecast10Days             = boost::make_shared<CForecastDays>(api, m_configuration);

      // if conditions are enabled
      if (m_configuration.IsConditionsIndividualKeywordsEnabled() || m_configuration.IsLiveConditionsEnabled())
      {
         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
      }

      // If astronomy is enabled
      if (m_configuration.IsAstronomyEnabled())
      {
         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
      }

      // If forecast is enabled
      if (m_configuration.IsForecast10DaysEnabled())
      {
         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
      }
   }
   catch (...)
   {
      // Informs Yadoms about the plugin actual state
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
   }

   std::cout << "CWeatherUnderground plugin is running..." << std::endl;

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
            try
            {
               weatherConditionsFinished = false;
               shared::CDataContainer returnData = SendUrlRequest(api, m_WeatherConditionsRequester->getUrl(), kEvtTimerRefreshWeatherConditions, weatherConditionsSendingRetry);
               m_WeatherConditionsRequester->parse(api, m_configuration, returnData);
               weatherConditionsFinished = true;
               api->getEventHandler().createTimer(kEvtPluginState, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            }
            catch(CRequestErrorException& )
            {}
         }
         break;
      case kEvtTimerRefreshAstronomy:
         {
            try
            {
               astronomyFinished = false;
               shared::CDataContainer returnData = SendUrlRequest(api, m_AstronomyRequester->getUrl(), kEvtTimerRefreshAstronomy, astronomySendingRetry);
               m_AstronomyRequester->parse(api, m_configuration, returnData);
               astronomyFinished = true;
               api->getEventHandler().createTimer(kEvtPluginState, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            }
            catch (CRequestErrorException&)
            {}
         }
         break;
      case kEvtTimerRefreshForecast10Days:
         {
            try
            {
               // TODO : A modifier !
               if (!weatherConditionsFinished)
                  m_Forecast10Days->setCityName(m_configuration.getLocalisation());
               else
                  m_Forecast10Days->setCityName(m_WeatherConditionsRequester->getCityName());

               forecast10daysFinished = false;

               shared::CDataContainer returnData = SendUrlRequest(api, m_Forecast10Days->getUrl(), kEvtTimerRefreshForecast10Days, forecast10daysSendingRetry);
               m_Forecast10Days->parse(api, m_configuration, returnData);
               forecast10daysFinished = true;
               api->getEventHandler().createTimer(kEvtPluginState, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            }
            catch (CRequestErrorException&)
            {}
         }
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            shared::CDataContainer returnData;
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());

            // Update configurations
            m_WeatherConditionsRequester->onUpdate(api, m_configuration);
            m_AstronomyRequester->onUpdate(api, m_configuration);
            m_Forecast10Days->onUpdate(api, m_configuration);

            if (m_configuration.IsConditionsIndividualKeywordsEnabled() || m_configuration.IsLiveConditionsEnabled())
               api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            if (m_configuration.IsAstronomyEnabled())
               api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            if (m_configuration.IsForecast10DaysEnabled())
               api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            break;
         }
      case kEvtPluginState:
         {
            yApi::historization::EPluginState newState = yApi::historization::EPluginState::kUnknown;

            // TODO : Voi comment estimer correctement l'état du plugin
            // estimate the state of the plugin
            if ((forecast10daysFinished || !m_configuration.IsForecast10DaysEnabled()) &&
                (astronomyFinished || !m_configuration.IsAstronomyEnabled()) &&
                weatherConditionsFinished || (!m_configuration.IsConditionsIndividualKeywordsEnabled() && !m_configuration.IsLiveConditionsEnabled()))
               newState = yApi::historization::EPluginState::kRunning;

            if (m_runningState != newState)
            {
               m_runningState = newState;

               if (m_runningState == yApi::historization::EPluginState::kRunning)
                  api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
         }
         break;
      default:
         std::cerr << "Unknown message id" << std::endl;
         break;
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
      shared::CDataContainer data = shared::CHttpMethods::SendGetRequest(url);

      ErrorAnswerHandler Response(api, data);

      if (Response.ContainError())
      {
         m_runningState = yApi::historization::EPluginState::kCustom;
         api->setPluginState(m_runningState, Response.getError());
         throw shared::exception::CException("Response contain error");
      }
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
         m_runningState = yApi::historization::EPluginState::kCustom;
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection"); 
         nbRetry = 0;
      }

      throw CRequestErrorException();
   }
}