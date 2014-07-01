#include "stdafx.h"
#include "LinuxSystemInformation.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "LinuxSystemMemoryLoad.h"
#include "LinuxSystemCPULoad.h"
#include "LinuxSystemYadomsCPULoad.h"
#include "LinuxSystemDiskUsage.h"
#include "LinuxSystemDisksList.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CLinuxSystemInformation)


CLinuxSystemInformation::CLinuxSystemInformation()
{
}

CLinuxSystemInformation::~CLinuxSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerSendMessage = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
};

void CLinuxSystemInformation::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CLinuxSystemInformation is starting...";

      CLinuxSystemMemoryLoad    MemoryLoad   ("MemoryLoad");
      CLinuxSystemCPULoad       CPULoad      ("CPULoad");
      CLinuxSystemYadomsCPULoad YadomsCPULoad("YadomsCPULoad");
      
      CLinuxSystemDisksList     DisksList;

      CPULoad.declareDevice(context);
      MemoryLoad.declareDevice(context);
      YadomsCPULoad.declareDevice(context);

      // Timer used to send a message periodically
      context->getEventHandler().createTimer(kEvtTimerSendMessage, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "LinuxSystemInformation plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerSendMessage:
            {
               // Timer used here to send a message periodically

               YADOMS_LOG(debug) << "LinuxSystem plugin :  Read a value...";

                  std::ostringstream ss;
                  std::ostringstream ss1;
                  std::ostringstream ss2;
                  std::ostringstream ss3;

                  try
                  {
                     ss << std::fixed << std::setprecision(2) << MemoryLoad.getValue();

                     YADOMS_LOG(debug) << "LinuxSystemInformation plugin :  Memory Load : " << ss.str();

                     MemoryLoad.historizeData(context);

                     ss1 << std::fixed << std::setprecision(2) << CPULoad.getValue();

                     YADOMS_LOG(debug) << "LinuxSystemInformation plugin :  CPU Load : " << ss1.str();

                     CPULoad.historizeData(context);

                     ss2 << std::fixed << std::setprecision(2) << YadomsCPULoad.getValue();

                     YADOMS_LOG(debug) << "LinuxSystemInformation plugin :  Yadoms CPU Load : " << ss2.str();

                     std::vector<std::string>::const_iterator DisksListIterator;
                     std::vector<std::string> TempList;
                     
                     TempList = DisksList.getList();

                     for(DisksListIterator=TempList.begin(); DisksListIterator!=TempList.end(); ++DisksListIterator)
	                  {
                        CLinuxSystemDiskUsage DiskUsage("DiskUsage", *DisksListIterator);

                        ss3 << std::fixed << std::setprecision(2) << DiskUsage.getValue();

                        YADOMS_LOG(debug) << "LinuxSystemInformation plugin :  Yadoms Disk Usage " << *DisksListIterator << " :" << ss3.str();
                     }
                  }
                  catch (boost::system::system_error& e)
                  {
                     YADOMS_LOG(error) << "LinuxSystemInformation plugin :  Exception" << e.what();
                     return;
                  }

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
      YADOMS_LOG(info) << "LinuxSystemInformation is stopping..."  << std::endl;
   }
}

