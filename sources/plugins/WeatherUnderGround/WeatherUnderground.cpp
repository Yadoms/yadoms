#include "stdafx.h"
#include "WeatherUnderground.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "WeatherConditions.h"
#include "Astronomy.h"
#include "ForecastDays.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)


CWeatherUnderground::CWeatherUnderground(): m_deviceName("WeatherUnderground")
{}

CWeatherUnderground::~CWeatherUnderground()
{}

// Event IDs
enum
{
   kEvtTimerRefreshWeatherConditions = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshAstronomy,
   kEvtTimerRefreshForecast10Days
};

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CWeatherUnderground is starting...";

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

	   // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically the Weather information
      context->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions , shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));

	   // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshAstronomy         , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically the Weather information
	   context->getEventHandler().createTimer(kEvtTimerRefreshAstronomy        , shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));

	   // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days    , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically the Weather information
	   context->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days   , shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));

	   if (!context->deviceExists(m_deviceName))
	   {
		  std::string m_URL = "www.wunderground.com/";

		  context->declareDevice(m_deviceName, m_URL);
	   }

      CWeatherConditions m_WeatherConditionsRequester( context, m_configuration, m_deviceName, "conditions.");
      CAstronomy m_AstronomyRequester                ( context, m_configuration, m_deviceName, "astronomy.");
      CForecastDays m_Forecast10Days                 ( context, m_configuration, m_deviceName, "forecast.");

      // the main loop
      YADOMS_LOG(debug) << "CWeatherUnderground plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshWeatherConditions:
            {
               YADOMS_LOG(debug) << "Refresh Weather Conditions";

			      if ( !m_WeatherConditionsRequester.Request( context ) )
					  m_WeatherConditionsRequester.Parse  ( context, m_configuration );

				  if (!m_WeatherConditionsRequester.IsModuleInFault ())
					  context->setPluginState(yApi::historization::EPluginState::kRunning);

               break;
            }
		   case kEvtTimerRefreshAstronomy:
            {
			      YADOMS_LOG(debug) << "Refresh Astronomy Information";

			      if ( !m_AstronomyRequester.Request( context ) )
			         m_AstronomyRequester.Parse  ( context, m_configuration );

				  if (!m_WeatherConditionsRequester.IsModuleInFault ())
					  context->setPluginState(yApi::historization::EPluginState::kRunning);

			      break;
            }
		 case kEvtTimerRefreshForecast10Days:
            {
			    YADOMS_LOG(debug) << "Refresh Forecast 10 Days Information";

			      if ( !m_Forecast10Days.Request( context ) )
				  {
                     m_Forecast10Days.SetCityName ( m_WeatherConditionsRequester.GetCityName() );
			         m_Forecast10Days.Parse  ( context, m_configuration );
				  }

				  if (!m_WeatherConditionsRequester.IsModuleInFault ())
					  context->setPluginState(yApi::historization::EPluginState::kRunning);

			      break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());

               m_WeatherConditionsRequester.OnUpdate ( context, m_configuration );
			   
               if (!m_WeatherConditionsRequester.Request( context ))
                  m_WeatherConditionsRequester.Parse  ( context, m_configuration );

               m_AstronomyRequester.OnUpdate ( m_configuration );

               if ( !m_AstronomyRequester.Request( context ))
                  m_AstronomyRequester.Parse  ( context, m_configuration );

			   m_Forecast10Days.OnUpdate ( context, m_configuration );

               m_Forecast10Days.SetCityName ( m_WeatherConditionsRequester.GetCityName());

			   if ( !m_Forecast10Days.Request( context ))
                  m_Forecast10Days.Parse  ( context, m_configuration );

			   if ( !m_WeatherConditionsRequester.IsModuleInFault () && 
				    !m_Forecast10Days.IsModuleInFault() &&
					!m_AstronomyRequester.IsModuleInFault() )
			   {
                  context->setPluginState(yApi::historization::EPluginState::kRunning);
			   }
               break;
            }

         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
         }
      };
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(information) << "CWeatherUnderground is stopping..."  << std::endl;
   }
}

void CWeatherUnderground::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(debug) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}