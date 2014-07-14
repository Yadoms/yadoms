#include "stdafx.h"
#include "WindowsSystemInformation.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "WindowsSystemMemoryLoad.h"
#include "WindowsSystemCPULoad.h"
#include "WindowsSystemYadomsCPULoad.h"
#include "WindowsSystemDiskUsage.h"
#include "WindowsSystemDisksList.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWindowsSystemInformation)


CWindowsSystemInformation::CWindowsSystemInformation()
{
}

CWindowsSystemInformation::~CWindowsSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshDiskAndMemory
};

void CWindowsSystemInformation::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CWindowsSystemInformation is starting...";

	  //declarations

      CWindowsSystemMemoryLoad    MemoryLoad   ("MemoryLoad");
      CWindowsSystemCPULoad       CPULoad      ("CPULoad");
      CWindowsSystemYadomsCPULoad YadomsCPULoad("YadomsCPULoad");
      
      CWindowsSystemDisksList     DisksList;

      std::vector<std::string>::const_iterator DisksListIterator;
      std::vector<std::string> TempList;

      std::vector<CWindowsSystemDiskUsage> DiskUsageList;
      
	  try
	  {
		  MemoryLoad.Initialize();
		  MemoryLoad.declareDevice(context);
	  }
	  catch (boost::thread_interrupted&)
	  {
		  YADOMS_LOG(debug) << "Device Memory is desactivated...";
	  }

	  try
	  {
		  CPULoad.Initialize();
		  CPULoad.declareDevice(context);
	  }
	  catch (boost::thread_interrupted&)
	  {
		  YADOMS_LOG(debug) << "Device CPU Load is desactivated...";
	  }

	  try
	  {
		  YadomsCPULoad.Initialize();
		  YadomsCPULoad.declareDevice(context);
	  }
	  catch (boost::thread_interrupted&)
	  {
		  YADOMS_LOG(debug) << "Device Yadoms CPU Load is desactivated...";
	  }

      TempList = DisksList.getList();
      int counterDisk = 0;

      for(DisksListIterator=TempList.begin(); DisksListIterator!=TempList.end(); ++DisksListIterator)
	   {
         std::ostringstream ssKeyword;
         std::ostringstream ssName;

         ssKeyword << "WindowsDiskUsage" << counterDisk;
         ssName << "DiskUsage" << counterDisk;
         CWindowsSystemDiskUsage DiskUsage(ssName.str(), *DisksListIterator, ssKeyword.str());

		 try
		 {
		    DiskUsage.Initialize();
			DiskUsage.declareDevice(context);
            DiskUsageList.push_back(DiskUsage);
			++counterDisk;
		 }
		 catch (boost::thread_interrupted&)
		 {
		    YADOMS_LOG(debug) << "One Disk Usage device is desactivated...";
		 }
      }

      // Timer used to read periodically CPU loads
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad      , shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));
      
      // Timer used to read periodically Disk Usage and Memory Load
      context->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(300));
      
      // the main loop
      YADOMS_LOG(debug) << "WindowsSystemInformation plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshCPULoad:
            {
               YADOMS_LOG(debug) << "WindowsSystem plugin :  Read CPU Loads";

                  std::ostringstream ss1;
                  std::ostringstream ss2;

                  try
                  {
                     ss1 << std::fixed << std::setprecision(2) << CPULoad.getValue();
                     YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  CPU Load : " << ss1.str();
                     CPULoad.historizeData(context);

                     ss2 << std::fixed << std::setprecision(2) << YadomsCPULoad.getValue();
                     YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Yadoms CPU Load : " << ss2.str();
                  }
                  catch (boost::system::system_error& e)
                  {
                     YADOMS_LOG(error) << "WindowsSystemInformation plugin :  Exception" << e.what();
                     return;
                  }

               break;
            }
         case kEvtTimerRefreshDiskAndMemory:
            {
               // Timer used here to send a message periodically

               YADOMS_LOG(debug) << "WindowsSystem plugin :  Read Memory and disk Usages";

                  std::ostringstream ss;
                  std::ostringstream ss3;

                  try
                  {
                     ss << std::fixed << std::setprecision(2) << MemoryLoad.getValue();

                     YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Memory Load : " << ss.str();

                     MemoryLoad.historizeData(context);

                     std::vector<CWindowsSystemDiskUsage>::iterator DisksListIterator;

                     for(DisksListIterator=DiskUsageList.begin(); DisksListIterator!=DiskUsageList.end(); ++DisksListIterator)
	                  { 
                        ss3 << std::fixed << std::setprecision(2) << (*DisksListIterator).getValue();
                        (*DisksListIterator).historizeData(context);
                        YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Yadoms Disk Usage " << (*DisksListIterator).getDriveName() << " :" << ss3.str();
                     }
                  }
                  catch (boost::system::system_error& e)
                  {
                     YADOMS_LOG(error) << "WindowsSystemInformation plugin :  Exception" << e.what();
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
      YADOMS_LOG(info) << "WindowsSystemInformation is stopping..."  << std::endl;
   }
}

