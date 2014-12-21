#include "stdafx.h"
#include "Wunderground.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "ForecastReceiver.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWunderground)


CWunderground::CWunderground(): m_deviceName("Wunderground")
{
}

CWunderground::~CWunderground()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshForecast = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
};

void CWunderground::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CWunderground is starting...";

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      m_APIKey = m_configuration.getAPIKey();
      m_Localisation = m_configuration.getLocalisation();

	  // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshForecast      , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically the Weather information
      context->getEventHandler().createTimer(kEvtTimerRefreshForecast      , shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));

	   if (!context->deviceExists(m_deviceName))
	   {
		  std::string m_URL = "www.wunderground.com/";

		  context->declareDevice(m_deviceName, m_URL);
	   }

	  CForecastReceiver m_ForecastRequester( context, m_deviceName, m_APIKey, m_Localisation );

      // the main loop
      YADOMS_LOG(debug) << "WUnderground plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshForecast:
            {
               YADOMS_LOG(debug) << "WUnderground : Refresh Forecast";

			   m_ForecastRequester.Request( context );
			   m_ForecastRequester.Parse  ( context );

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());

			   m_ForecastRequester.OnUpdate ( m_deviceName, m_APIKey, m_Localisation );
			   
			   m_ForecastRequester.Request( context );
			   m_ForecastRequester.Parse  ( context );
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
      YADOMS_LOG(information) << "CWunderground is stopping..."  << std::endl;
   }
}

void CWunderground::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(debug) << "Configuration was updated...";
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);

   m_APIKey = m_configuration.getAPIKey();
   m_Localisation = m_configuration.getLocalisation();
}