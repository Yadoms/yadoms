#include "stdafx.h"
#include "WeatherUnderground.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "WeatherConditions.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)


CWeatherUnderground::CWeatherUnderground(): m_deviceName("WeatherUnderground")
{}

CWeatherUnderground::~CWeatherUnderground()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshForecast = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
};

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CWeatherUnderground is starting...";

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

	  // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshForecast      , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically the Weather information
      context->getEventHandler().createTimer(kEvtTimerRefreshForecast      , shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));

	   if (!context->deviceExists(m_deviceName))
	   {
		  std::string m_URL = "www.wunderground.com/";

		  context->declareDevice(m_deviceName, m_URL);
	   }

	  CWeatherConditions m_WeatherConditionsRequester( context, m_configuration);

      // the main loop
      YADOMS_LOG(debug) << "CWeatherUnderground plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshForecast:
            {
               YADOMS_LOG(debug) << "CWeatherUnderground : Refresh Weather Conditions";

			   m_WeatherConditionsRequester.Request( context );
			   m_WeatherConditionsRequester.Parse  ( context, m_configuration, m_deviceName );

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());

			   m_WeatherConditionsRequester.OnUpdate ( m_configuration );
			   
			   m_WeatherConditionsRequester.Request( context );
			   m_WeatherConditionsRequester.Parse  ( context, m_configuration, m_deviceName );
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
   YADOMS_LOG(debug) << "Configuration was updated...";
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}