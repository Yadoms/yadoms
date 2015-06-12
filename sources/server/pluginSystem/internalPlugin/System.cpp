#include "stdafx.h"
#include "System.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/historization/Historizers.h>
#include "../../IApplicationStopHandler.h"
#include <shared/ServiceLocator.h>

namespace pluginSystem {
   namespace internalPlugin {

   CSystem::CSystem()
   {
   }

   CSystem::~CSystem()
   {
   }


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

         // Device creation if needed
         if (!context->deviceExists("system"))
            context->declareDevice("system", "yadoms system");

         // Keywords creation if needed
         if (!context->keywordExists("system", keywordShutdown.getKeyword()))
            context->declareKeyword("system", keywordShutdown);

         if (!context->keywordExists("system", keywordRestart.getKeyword()))
            context->declareKeyword("system", keywordRestart);

         boost::shared_ptr<IApplicationStopHandler> applicationStopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();

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
               default:
                  break;
            }
         };

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




} } // namespace pluginSystem::internalPlugin


