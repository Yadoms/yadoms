#include "stdafx.h"
#include "System.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/historization/Historizers.h>


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

         //D?clare all device/keywords
			yApi::historization::CShutdown keywordShutdown("shutdown");
			yApi::historization::CRestart keywordRestart("restart");

         //création du device system si necessaire
         if (!context->deviceExists("system"))
            context->declareDevice("system", "yadoms system");

         //création des keyword si necessaire
         if (!context->keywordExists("system", keywordShutdown.getKeyword()))
            context->declareKeyword("system", keywordShutdown);

         if (!context->keywordExists("system", keywordRestart.getKeyword()))
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


