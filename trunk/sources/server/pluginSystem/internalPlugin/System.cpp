#include "stdafx.h"
#include "System.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/plugin/yPluginApi/historization/Historizers.h>
#include "../../IApplicationStopHandler.h"
#include <shared/ServiceLocator.h>
#include "../../automation/script/IObjectFactory.h"
#include "../../automation/script/DayLight.h"

namespace pluginSystem {
   namespace internalPlugin {

   CSystem::CSystem()
   {
   }

   CSystem::~CSystem()
   {
   }

   // Event IDs
   enum
   {
      kEvtTimerForSunset = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
      kEvtTimerForSunrise,
      kEvtNewDay
   };

   void CSystem::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
   {
      try
      {
         YADOMS_LOG_CONFIGURE("System");

         // the main loop
         YADOMS_LOG(debug) << "System is running...";

         // Declare all device/keywords
         static const std::string& systemDevice("system");
         yApi::historization::CEvent keywordShutdown("shutdown");
         yApi::historization::CEvent keywordRestart("restart");
         yApi::historization::CSwitch keywordDaylight("daylight", shared::plugin::yPluginApi::EKeywordAccessMode::kGet);

         // Device creation if needed
         if (!context->deviceExists(systemDevice))
            context->declareDevice(systemDevice, "yadoms system");

         // Keywords creation if needed
         if (!context->keywordExists(systemDevice, keywordShutdown.getKeyword()))
            context->declareKeyword(systemDevice, keywordShutdown);

         if (!context->keywordExists(systemDevice, keywordRestart.getKeyword()))
            context->declareKeyword(systemDevice, keywordRestart);

         if (!context->keywordExists(systemDevice, keywordDaylight.getKeyword()))
            context->declareKeyword(systemDevice, keywordDaylight);

         boost::shared_ptr<IApplicationStopHandler> applicationStopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();

         // Create daylight timers and set keyword first state
         keywordDaylight.set(createDayLightTimers(context->getEventHandler()));
         context->historizeData(systemDevice, keywordDaylight);

         while (1)
         {
            // Wait for an event
            switch (context->getEventHandler().waitForEvents())
            {
               case yApi::IYPluginApi::kEventDeviceCommand:
               {
                  // Command was received from Yadoms
                  boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

                  if (boost::iequals(command->getKeyword(), keywordShutdown.getKeyword()))
                  {
                     YADOMS_LOG(information) << "Shutdown the system";
                     applicationStopHandler->requestToStop(IApplicationStopHandler::kStopSystem);
                  }
                  else if (boost::iequals(command->getKeyword(), keywordRestart.getKeyword()))
                  {
                     YADOMS_LOG(information) << "Restart the system";
                     applicationStopHandler->requestToStop(IApplicationStopHandler::kRestartSystem);
                  }
                  else
                  {
                     YADOMS_LOG(warning) << "Received command for unknown keyword from Yadoms : " << command->toString();
                  }
                  break;
               }
               case kEvtTimerForSunrise:
               {
                  YADOMS_LOG(information) << "Sun is rising. Have a good day !";
                  keywordDaylight.set(true);
                  context->historizeData(systemDevice, keywordDaylight);
                  break;
               }
               case kEvtTimerForSunset:
               {
                  YADOMS_LOG(information) << "Sun goes down. Have a good night !";
                  keywordDaylight.set(false);
                  context->historizeData(systemDevice, keywordDaylight);
                  break;
               }
               case kEvtNewDay:
               {
                  YADOMS_LOG(information) << "New day";
                  createDayLightTimers(context->getEventHandler());
                  break;
               }
               default:
                  break;
            }
         }

      }
      // Plugin must catch this end-of-thread exception to make its cleanup.
      // If no cleanup is necessary, still catch it, or Yadoms will consider
      // as a plugin failure.
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(information) << "Thread is stopping...";
      }
      catch (shared::exception::CException & ex)
      {
         YADOMS_LOG(fatal) << "The Yadoms Sytem plugin fails. Unknown expcetion : " << ex.what();
      }
   }

   bool CSystem::createDayLightTimers(shared::event::CEventHandler& eventHandler) const
   {
      boost::posix_time::ptime now = shared::event::now();

      boost::shared_ptr<automation::script::IDayLight> dayLightProvider;
      try
      {
         dayLightProvider = shared::CServiceLocator::instance().get<automation::script::IObjectFactory>()->getDayLight();
      }
      catch (shared::exception::CEmptyResult&)
      {
         YADOMS_LOG(error) << "Sunrise and sunset time are not available (did you provide your location ?)";
         return false;
      }

      boost::posix_time::ptime sunrise = dayLightProvider->sunriseTime();
      boost::posix_time::ptime sunset = dayLightProvider->sunsetTime();

      createSunEventTimer(eventHandler, kEvtTimerForSunrise, now, sunrise);
      createSunEventTimer(eventHandler, kEvtTimerForSunset, now, sunset);


      YADOMS_LOG(information) << "Sunrise will be at " << automation::script::CDayLight::formatSunEventTime(sunrise)
         << " and sunset at " << automation::script::CDayLight::formatSunEventTime(sunset);

      // Create another time point to re-compute new sun events on start of next day
      boost::posix_time::ptime tomorrowMidnight(now.date() + boost::gregorian::days(1));
      eventHandler.createTimePoint(kEvtNewDay, tomorrowMidnight);

      return (now >= sunrise && now < sunset);
   }

   void CSystem::createSunEventTimer(shared::event::CEventHandler& eventHandler, int daylightEventTimerId,
      const boost::posix_time::ptime& now, const boost::posix_time::ptime& daylightEventTime)
   {
      try
      {
         // Be careful, createTimePoint also does a comparison to now(), which can be a bit different
         // (if CPU is heavy loaded for example), and rise an exception if time point is considered in the future.
         if (now < daylightEventTime)
            eventHandler.createTimePoint(daylightEventTimerId, daylightEventTime);
      }
      catch (shared::exception::COutOfRange&)
      {
         // It can just occur when this method is executed at the exact same time as the createTimePoint.
         // In this case, immediatly signal event
         eventHandler.postEvent(daylightEventTimerId);
      }
   }


} } // namespace pluginSystem::internalPlugin


