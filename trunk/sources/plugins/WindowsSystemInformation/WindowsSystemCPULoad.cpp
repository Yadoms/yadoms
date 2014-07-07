#include "stdafx.h"
#include "WindowsSystemCPULoad.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>
#include <pdh.h>

#pragma comment(lib, "pdh.lib")

CWindowsSystemCPULoad::CWindowsSystemCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0), m_Capacity("cpuload"), m_Keyword("WindowsCPULoad")
{
   PDH_STATUS Status;

   // Create the Query
   Status = PdhOpenQuery(NULL, NULL, &m_cpuQuery);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhOpenQuery failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   // Add the selected counter to the query
#if (_WIN32_WINDOWS >= 0x0600 || _WIN32_WINNT >= 0x0600 || (defined(NTDDI_VERSION) && NTDDI_VERSION >= NTDDI_VISTA))
   Status = PdhAddEnglishCounter(m_cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &m_cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddEnglishCounter failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
#else
   //TODO : A compléter pour que cela fonctionne pour d'autres langues -> PdhEnumObjects
   Status = PdhAddCounter(m_cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &m_cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddCounter failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
#endif

   Status = PdhCollectQueryData(m_cpuQuery);
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

   Status = PdhCloseQuery(m_cpuQuery);
   
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCloseQuery failed with status:"; 
      Message << GetLastError();

      YADOMS_LOG(debug) << Message.str();
   }
}

const std::string& CWindowsSystemCPULoad::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CWindowsSystemCPULoad::getCapacity() const
{
   return m_Capacity;
}

const std::string& CWindowsSystemCPULoad::getKeyword() const
{
   return m_Keyword;
}

void CWindowsSystemCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, getKeyword(), getCapacity(), yApi::kReadOnly, yApi::kDecimal, yApi::CStandardUnits::Percent);
}

void CWindowsSystemCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   
   context->historizeData(m_deviceId, getKeyword(), m_CPULoad);
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

   Status = PdhCollectQueryData(m_cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 | PDH_FMT_NOSCALE, &CounterType, &counterVal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhGetFormattedCounterValue failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   m_CPULoad = counterVal.doubleValue;

   return counterVal.doubleValue;
}


