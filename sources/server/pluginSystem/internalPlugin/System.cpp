#include "stdafx.h"
#include "System.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include "ShutdownCommand.h"
#include "RestartCommand.h"

namespace pluginSystem {
   namespace internalPlugin {

   CSystem::CSystem()
   {
   }

   CSystem::~CSystem()
   {
   }


   void CSystem::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
   {
      try
      {
         YADOMS_LOG_CONFIGURE("System");

         // the main loop
         YADOMS_LOG(debug) << "System is running...";

         //Déclare all device/keywords
         CShutdownCommand keywordShutdown("shutdown");
         CRestartCommand keywordRestart("restart");

         context->declareDevice("system", "yadoms system");
         context->declareKeyword("system", keywordShutdown);
         context->declareKeyword("system", keywordRestart);

         while (1)
         {
            // Wait for an event
            switch (context->getEventHandler().waitForEvents())
            {
               case yApi::IYadomsApi::kEventDeviceCommand:
               {
                  // Command was received from Yadoms
                  boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

                  if (boost::iequals(command->getKeyword(), keywordShutdown.getKeyword()))
                     YADOMS_LOG(info) << "Shutdown the system";
                  else if (boost::iequals(command->getKeyword(), keywordRestart.getKeyword()))
                     YADOMS_LOG(info) << "Restart the system";
                  else
                     YADOMS_LOG(info) << "unknown keyword";
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
      }
      catch (shared::exception::CException & ex)
      {
         YADOMS_LOG(fatal) << "The Yadoms Sytem plugin fails. Unknown expcetion : " << ex.what();
      }
   }




} } // namespace pluginSystem::internalPlugin


