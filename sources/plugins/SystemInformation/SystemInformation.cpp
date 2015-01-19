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

      // Device declaration, if needed
      if (!context->deviceExists(m_deviceName))
      {
         context->declareDevice(m_deviceName, "SystemInformation");
      }

      CSystemFactory Factory (context, m_deviceName);

      // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad      , shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically CPU loads
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad      , shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

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
			   Factory.OnSpeedUpdate (context);

               break;
            }
         case kEvtTimerRefreshDiskAndMemory:
            {
			   Factory.OnSlowUpdate (context);
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

