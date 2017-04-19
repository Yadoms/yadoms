#include "stdafx.h"
#include "System.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/historization/Historizers.h>
#include <tools/OperatingSystem.h>

namespace pluginSystem {
   namespace internalPlugin {

   CSystem::CSystem()
   {
   }

   CSystem::~CSystem()
   {
   }

   void CSystem::doWork(boost::shared_ptr<yApi::IYPluginApi> api,
                        shared::event::CEventHandler& eventHandler)
   {
      try
      {
         YADOMS_LOG_CONFIGURE("InternalPlugin");

         // the main loop
         YADOMS_LOG(debug) << "InternalPlugin is running...";

         // Declare all device/keywords
         static const std::string& systemDevice("system");
         auto keywordShutdown(boost::make_shared<yApi::historization::CEvent>("shutdown"));
         auto keywordRestart(boost::make_shared<yApi::historization::CEvent>("restart"));
         std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> > keywords({ keywordShutdown, keywordRestart });

         // Device creation if needed
         if (!api->deviceExists(systemDevice))
            api->declareDevice(systemDevice,
                               "system",
                               "yadoms system",
                               keywords);

         while (1)
         {
            // Wait for an event
            switch (eventHandler.waitForEvents())
            {
               case yApi::IYPluginApi::kEventDeviceCommand:
               {
                  // Command was received from Yadoms
                  auto command = eventHandler.getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

                  if (boost::iequals(command->getKeyword(), keywordShutdown->getKeyword()))
                  {
                     YADOMS_LOG(information) << "Shutdown the system";
                     tools::COperatingSystem::shutdown();
                  }
                  else if (boost::iequals(command->getKeyword(), keywordRestart->getKeyword()))
                  {
                     YADOMS_LOG(information) << "Restart the system";
                     tools::COperatingSystem::shutdown(true);
                  }
                  else
                  {
                     YADOMS_LOG(warning) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command);
                  }
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

} } // namespace pluginSystem::internalPlugin


