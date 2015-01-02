#include "stdafx.h"
#include "RaspBerryPiSystemInformation.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "RaspBerryPiSystemMemoryLoad.h"
#include "RaspBerryPiSystemCPULoad.h"
#include "RaspBerryPiSystemYadomsCPULoad.h"
#include "RaspBerryPiSystemDiskUsage.h"
#include "RaspBerryPiSystemDisksList.h"
#include "RaspberryPiTemperatureSensor.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CRaspBerryPiSystemInformation)


   CRaspBerryPiSystemInformation::CRaspBerryPiSystemInformation() : m_deviceName("System")
{
}

CRaspBerryPiSystemInformation::~CRaspBerryPiSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad =  yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshDiskAndMemory
};

void CRaspBerryPiSystemInformation::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {

      YADOMS_LOG(debug) << "CRaspBerryPiSystemInformation is starting...";

      CRaspBerryPiSystemMemoryLoad    MemoryLoad     (m_deviceName);
      CRaspBerryPiSystemCPULoad       CPULoad        (m_deviceName);
      CRaspBerryPiSystemYadomsCPULoad YadomsCPULoad  (m_deviceName);
      CRaspberryPiTemperatureSensor   TemperatureCPU (m_deviceName);

      CRaspBerryPiSystemDisksList     DisksList;

      // Device and keywords declaration, if needed
      if (!context->deviceExists(m_deviceName))
      {
         context->declareDevice(m_deviceName, "System memory load");
         MemoryLoad.declareKeywords(context);
         CPULoad.declareKeywords(context);
         YadomsCPULoad.declareKeywords(context);
      }	  

      std::vector<std::string> TempList;
      std::list<CRaspBerryPiSystemDiskUsage> DiskUsageList;

      TempList = DisksList.getList();

      for(std::vector<std::string>::const_iterator disksListIterator = TempList.begin(); disksListIterator != TempList.end(); ++ disksListIterator)
      {
         std::string diskKeywordName = disksListIterator->substr(5, 4) + "_DiskUsage";
         YADOMS_LOG(debug) << "Disk Name:" << diskKeywordName;
         CRaspBerryPiSystemDiskUsage DiskUsage(m_deviceName, *disksListIterator, diskKeywordName);
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
      YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerRefreshCPULoad:
            {
               YADOMS_LOG(debug) << "RaspBerryPiSystem plugin :  Read CPU Loads";

               CPULoad.read();
               YadomsCPULoad.read();
               TemperatureCPU.read();

               CPULoad.historizeData(context);
               YadomsCPULoad.historizeData(context);
               TemperatureCPU.historizeData(context);
            }
            break;
         case kEvtTimerRefreshDiskAndMemory:
            {
               YADOMS_LOG(debug) << "RaspBerryPiSystem plugin :  Read Memory and disk Usages";

               MemoryLoad.read();
               MemoryLoad.historizeData(context);

               for(std::list<CRaspBerryPiSystemDiskUsage>::iterator disksListIterator=DiskUsageList.begin(); disksListIterator!=DiskUsageList.end(); ++disksListIterator)
               {
                  disksListIterator->read();
                  disksListIterator->historizeData(context);
               }
            }
             break;
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
      YADOMS_LOG(information) << "RaspBerryPiSystemInformation is stopping..."  << std::endl;
   }
}

