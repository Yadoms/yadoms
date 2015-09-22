#include "stdafx.h"
#include "System.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/historization/Historizers.h>
#include "../../IApplicationStopHandler.h"
#include <shared/ServiceLocator.h>
#include <server/automation/script/IDayLight.h>

namespace pluginSystem {
   namespace internalPlugin {

   CSystem::CSystem(boost::shared_ptr<automation::script::IDayLight> dayLightProvider)
      :m_dayLightProvider(dayLightProvider)
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
         yApi::historization::CEvent keywordShutdown("shutdown");
         yApi::historization::CEvent keywordRestart("restart");
         yApi::historization::CSwitch keywordDaylight("daylight", shared::plugin::yPluginApi::EKeywordAccessMode::kGet); // TODO Ajouter KWs pour calcul offsets par rapport au daylight

         // Device creation if needed
         if (!context->deviceExists("system"))
            context->declareDevice("system", "yadoms system");

         // Keywords creation if needed
         if (!context->keywordExists("system", keywordShutdown.getKeyword()))
            context->declareKeyword("system", keywordShutdown);

         if (!context->keywordExists("system", keywordRestart.getKeyword()))
            context->declareKeyword("system", keywordRestart);

         if (!context->keywordExists("system", keywordDaylight.getKeyword()))
            context->declareKeyword("system", keywordDaylight);

         boost::shared_ptr<IApplicationStopHandler> applicationStopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();

         createDayLightTimers(context->getEventHandler());
         //TODO set keywordDaylight à la bonne valeur

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
                  break;
               }
               case kEvtTimerForSunset:
               {
                  YADOMS_LOG(information) << "Sun goes down. Have a good night !";
                  keywordDaylight.set(false);
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

   void CSystem::createDayLightTimers(shared::event::CEventHandler& eventHandler) const
   {
      boost::posix_time::ptime now = shared::event::now();

      // TODO mettre dans une fonction pour éviter la répétition ?
      //TODO réutiliser le time point pour éviter des fuites mémoire si certains sont malgré les précautions prises, posés dans le passé

      boost::posix_time::ptime sunrise, sunset;
      try
      {
         // Be careful, createTimePoint also does a comparison to now(), which can be a bit different
         // (if CPU is heavy loaded for example), and rise an exception if time point is considered in the future.
         sunrise = m_dayLightProvider->sunriseTime();
         if (now < sunrise)
            eventHandler.createTimePoint(kEvtTimerForSunrise, sunrise);
      }
      catch (shared::exception::COutOfRange&)
      {
         // It can just occur when createDayLightTimers is execute at the exact same time as the createTimePoint.
         // In this case, immediatly signal event
         eventHandler.postEvent(kEvtTimerForSunrise);
      }

      try
      {
         // Be careful, createTimePoint also does a comparison to now(), which can be a bit different
         // (if CPU is heavy loaded for example), and rise an exception if time point is considered in the future.
         sunset = m_dayLightProvider->sunsetTime();
         if (now < sunset)
            eventHandler.createTimePoint(kEvtTimerForSunset, sunset);
      }
      catch (shared::exception::COutOfRange&)
      {
         // It can just occur when createDayLightTimers is execute at the exact same time as the createTimePoint.
         // In this case, immediatly signal event
         eventHandler.postEvent(kEvtTimerForSunset);
      }

      YADOMS_LOG(information) << "Sunrise will be at " << sunrise.time_of_day().hours() << ":" << sunrise.time_of_day().minutes()
         << " and sunset at " << sunset.time_of_day().hours() << ":" << sunset.time_of_day().minutes();

      // Create another time point to re-compute new sun events on start of next day
      boost::posix_time::ptime tomorrow(now.date() + boost::gregorian::days(1));
      eventHandler.createTimePoint(kEvtNewDay, tomorrow);
   }



} } // namespace pluginSystem::internalPlugin


