#include "stdafx.h"
#include "SystemInformation.h"
#include <SystemFactory.h>
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CSystemInformation)


CSystemInformation::CSystemInformation(): m_deviceName("SystemInformation")
{}

CSystemInformation::~CSystemInformation()
{}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshDiskAndMemory
};

void CSystemInformation::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "SystemInformation is starting...";

      m_configuration.initializeWith(context->getConfiguration());

      // Device declaration, if needed
      if (!context->deviceExists(m_deviceName))
         context->declareDevice(m_deviceName, "SystemInformation");

	  shared::CDataContainer details;
	  details.set("provider", "SystemInformation");
	  details.set("shortProvider", "si");

      CSystemFactory Factory (context, m_deviceName, m_configuration, details);

      // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad      , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically CPU loads
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad      , shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));

      // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically Disk Usage and Memory Load
      context->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(300));

      // the main loop
      YADOMS_LOG(debug) << "SystemInformation plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshCPULoad:
            {
			   Factory.OnSpeedUpdate ( context );

               break;
            }
         case kEvtTimerRefreshDiskAndMemory:
            {
			   Factory.OnSlowUpdate ( context, m_configuration );
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               context->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());
               Factory.OnConfigurationUpdate (context, m_configuration, details );
               context->setPluginState(yApi::historization::EPluginState::kRunning);
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
      YADOMS_LOG(information) << "SystemInformation is stopping..."  << std::endl;
   }
}

void CSystemInformation::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(debug) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}
