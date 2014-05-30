#include "stdafx.h"
#include "WindowsSystemCPULoad.h"
#include "pdh.h"
#include <shared/exception/Exception.hpp>

#pragma comment(lib, "pdh.lib")

CWindowsSystemCPULoad::CWindowsSystemCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0)
{
   PDH_STATUS Status;

   // Create the Query

   Status = PdhOpenQuery(NULL, NULL, &cpuQuery);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhOpenQuery failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   // Add the selected counter to the query
   Status = PdhAddCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddCounter failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhCollectQueryData(cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
}

CWindowsSystemCPULoad::~CWindowsSystemCPULoad()
{
   PDH_STATUS Status;

   Status = PdhCloseQuery(cpuQuery);
   
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCloseQuery failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
}

const std::string& CWindowsSystemCPULoad::getDeviceId() const
{
   return m_deviceId;
}

double CWindowsSystemCPULoad::getValue() /*const*/
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
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 | PDH_FMT_NOSCALE, &CounterType, &counterVal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhGetFormattedCounterValue failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   return counterVal.doubleValue;
}


