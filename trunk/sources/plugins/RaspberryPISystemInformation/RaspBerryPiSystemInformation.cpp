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


CRaspBerryPiSystemInformation::CRaspBerryPiSystemInformation() : m_DeviceName("System")
{
}

CRaspBerryPiSystemInformation::~CRaspBerryPiSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad =  yApi::IYadomsApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshDiskAndMemory
};

void CRaspBerryPiSystemInformation::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      CRaspBerryPiSystemMemoryLoad    MemoryLoad     (m_DeviceName);
      CRaspBerryPiSystemCPULoad       CPULoad        (m_DeviceName);
      CRaspBerryPiSystemYadomsCPULoad YadomsCPULoad  (m_DeviceName);
      CRaspberryPITemperatureSensor   TemperatureCPU (m_DeviceName);
	  
      CRaspBerryPiSystemDisksList     DisksList;

      std::vector<std::string>::const_iterator DisksListIterator;
      std::vector<std::string> TempList;

      std::list<CRaspBerryPiSystemDiskUsage> DiskUsageList;
                     
      TempList = DisksList.getList();

      int counterDisk = 0;

      for(DisksListIterator=TempList.begin(); DisksListIterator!=TempList.end(); ++DisksListIterator)
	   {
         std::ostringstream ssKeyword;

         ssKeyword << "DiskUsage" << counterDisk;
         CRaspBerryPiSystemDiskUsage DiskUsage(m_DeviceName, *DisksListIterator, ssKeyword.str());

         DiskUsage.declareDevice(context);
         DiskUsageList.push_back(DiskUsage);
         ++counterDisk;
      }

      CPULoad.declareDevice(context);
      MemoryLoad.declareDevice(context);
      YadomsCPULoad.declareDevice(context);
      TemperatureCPU.declareDevice(context);

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
               YADOMS_LOG(debug) << "RaspBerryPiSystem plugin :  Read a value...";

                  std::ostringstream ss1;
                  std::ostringstream ss2;
		  std::ostringstream ss3;

                  try
                  {
                     ss1 << std::fixed << std::setprecision(2) << CPULoad.getValue();
                     CPULoad.historizeData(context);
                     YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin :  CPU Load : " << ss1.str();

                     ss2 << std::fixed << std::setprecision(2) << YadomsCPULoad.getValue();
                     YadomsCPULoad.historizeData(context);
                     YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin :  Yadoms CPU Load : " << ss2.str();

                     ss3 << std::fixed << std::setprecision(2) << TemperatureCPU.getValue();
                     TemperatureCPU.historizeData(context);
                     YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin :  CPU Temperature : " << ss3.str() << "°C";				 
                  }
                  catch (boost::system::system_error& e)
                  {
                     YADOMS_LOG(error) << "RaspBerryPiSystemInformation plugin :  Exception" << e.what();
                     return;
                  }
               }
               break;
         case kEvtTimerRefreshDiskAndMemory:
            {
               YADOMS_LOG(debug) << "RaspBerryPiSystem plugin :  Read a value...";

                  std::ostringstream ss;

                  try
                  {
                     ss << std::fixed << std::setprecision(2) << MemoryLoad.getValue();

                     YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin :  Memory Load : " << ss.str();

                     MemoryLoad.historizeData(context);

                     std::list<CRaspBerryPiSystemDiskUsage>::iterator DisksListIterator;

                     for(DisksListIterator=DiskUsageList.begin(); DisksListIterator!=DiskUsageList.end(); ++DisksListIterator)
	                  {
                        std::ostringstream ss3;
                        ss3 << std::fixed << std::setprecision(2) << (*DisksListIterator).getValue();
                        (*DisksListIterator).historizeData(context);
                        YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin :  Yadoms Disk Usage " << (*DisksListIterator).getDriveName() << " :" << ss3.str();
                     }

                  }
                  catch (boost::system::system_error& e)
                  {
                     YADOMS_LOG(error) << "RaspBerryPiSystemInformation plugin :  Exception" << e.what();
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
      YADOMS_LOG(info) << "RaspBerryPiSystemInformation is stopping..."  << std::endl;
   }
}

