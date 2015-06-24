#include "stdafx.h"
#include "WindowsSystemCPULoad.h"
#include <shared/Log.h>
#include "TCHAR.h"
#include "pdh.h"

#pragma comment(lib, "pdh.lib")

//TODO : Créer une exception

CWindowsSystemCPULoad::CWindowsSystemCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0)
{
   PDH_STATUS Status;

   // Create the Query

   Status = PdhOpenQuery(NULL, NULL, &cpuQuery);

   if (Status != ERROR_SUCCESS) 
   {
      YADOMS_LOG(debug) << "PdhOpenQuery failed with status:" << Status;
      throw ;
   }

   // Add the selected counter to the query
   Status = PdhAddCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      YADOMS_LOG(debug) << "PdhAddCounter failed with status:" << Status;
      throw ;
   }

   Status = PdhCollectQueryData(cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      YADOMS_LOG(debug) << "PdhCollectQueryData failed with status:" << Status;
      throw ;
   }   
}

CWindowsSystemCPULoad::~CWindowsSystemCPULoad()
{
   PdhCloseQuery(cpuQuery);
}

const std::string& CWindowsSystemCPULoad::getDeviceId() const
{
   return m_deviceId;
}

double CWindowsSystemCPULoad::getValue() const
{
   PDH_FMT_COUNTERVALUE counterVal;
   PDH_STATUS Status;
   DWORD CounterType;

   // Most counters require two sample values to display a formatted value.
   // PDH stores the current sample value and the previously collected
   // sample value. This call retrieves the first value that will be used
   // by PdhGetFormattedCounterValue in the first iteration of the loop
   // Note that this value is lost if the counter does not require two
   // values to compute a displayable value.

   Status = PdhCollectQueryData(cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      YADOMS_LOG(debug) << "PdhCollectQueryData failed with status:" << Status;
      throw ;
   }

   Status = PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 | PDH_FMT_NOSCALE, &CounterType, &counterVal);

   if (Status != ERROR_SUCCESS) 
   {
      YADOMS_LOG(debug) << "PdhGetFormattedCounterValue failed with status:" << Status;
      throw ;
   }

   return counterVal.doubleValue;
}


