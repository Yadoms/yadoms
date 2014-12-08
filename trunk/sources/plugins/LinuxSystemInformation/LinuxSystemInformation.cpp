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


CLinuxSystemInformation::CLinuxSystemInformation() : m_deviceName("System")
{
}

CLinuxSystemInformation::~CLinuxSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad =  yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshDiskAndMemory
};

void CLinuxSystemInformation::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CLinuxSystemInformation is starting...";

      CLinuxSystemMemoryLoad    MemoryLoad   (m_deviceName);
      CLinuxSystemCPULoad       CPULoad      (m_deviceName);
      CLinuxSystemYadomsCPULoad YadomsCPULoad(m_deviceName);

      CLinuxSystemDisksList     DisksList;

      // Device and keywords declaration, if needed
      if (!context->deviceExists(m_deviceName))
      {
         context->declareDevice(m_deviceName, "System memory load");
         MemoryLoad.declareKeywords(context);
         CPULoad.declareKeywords(context);
         YadomsCPULoad.declareKeywords(context);
      }	  

      std::vector<std::string> TempList;

      std::list<CLinuxSystemDiskUsage> DiskUsageList;

      TempList = DisksList.getList();
	  
      for(std::vector<std::string>::const_iterator disksListIterator = TempList.begin(); disksListIterator != TempList.end(); ++ disksListIterator)
      {
         std::string diskKeywordName = disksListIterator->substr(5, 4) + "_DiskUsage";
         YADOMS_LOG(debug) << "Disk Name:" << diskKeywordName;
         CLinuxSystemDiskUsage DiskUsage(m_deviceName, *disksListIterator, diskKeywordName);
         DiskUsageList.push_back(DiskUsage);
         if (!context->keywordExists(m_deviceName, diskKeywordName))
            DiskUsage.declareKeywords(context);
      }	  
	  
      // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad, shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically CPU loads
      context->getEventHandler().createTimer(kEvtTimerRefreshCPULoad, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // Event to be sent immediately for the first value
      context->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kOneShot , boost::posix_time::seconds(0));
      // Timer used to read periodically Disk Usage and Memory Load
      context->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(300));

      // the main loop
      YADOMS_LOG(debug) << "LinuxSystemInformation plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshCPULoad:
            {
               YADOMS_LOG(debug) << "LinuxSystem plugin :  Read CPU Loads";

               CPULoad.read();
               YadomsCPULoad.read();

               CPULoad.historizeData(context);
               YadomsCPULoad.historizeData(context);
			   
			   break;
            }
			
         case kEvtTimerRefreshDiskAndMemory:
            {
               YADOMS_LOG(debug) << "LinuxSystem plugin :  Read Memory and disk Usages";

               MemoryLoad.read();
               MemoryLoad.historizeData(context);

               for(std::list<CLinuxSystemDiskUsage>::iterator disksListIterator=DiskUsageList.begin(); disksListIterator!=DiskUsageList.end(); ++disksListIterator)
               {
                  disksListIterator->read();
                  disksListIterator->historizeData(context);
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

