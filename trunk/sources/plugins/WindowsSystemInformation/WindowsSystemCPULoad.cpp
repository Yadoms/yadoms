#include "stdafx.h"
#include "WindowsSystemCPULoad.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>
#include <pdh.h>
#include <pdhmsg.h>

#pragma comment(lib, "pdh.lib")

//Error Messages could be obtained with this function:
// http://msdn.microsoft.com/en-us/library/aa373046%28VS.85%29.aspx

CWindowsSystemCPULoad::CWindowsSystemCPULoad(const std::string & device)
   :m_device(device), m_keyword("CPULoad"), m_InitializeOk(false)
{
   Initialize();
}

void CWindowsSystemCPULoad::Initialize()
{
   PDH_STATUS Status;

   // Create the Query
   Status = PdhOpenQuery(NULL, NULL, &m_cpuQuery);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhOpenQuery failed with status:"; 
      Message << std::hex << GetLastError();
      m_InitializeOk = false;
      throw shared::exception::CException ( Message.str() );
   }

   // Add the selected counter to the query
#if (_WIN32_WINDOWS >= 0x0600 || _WIN32_WINNT >= 0x0600 || (defined(NTDDI_VERSION) && NTDDI_VERSION >= NTDDI_VISTA))
   Status = PdhAddEnglishCounter(m_cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &m_cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddEnglishCounter failed with status:"; 
      Message << std::hex << GetLastError();
      m_InitializeOk = false;
      throw shared::exception::CException ( Message.str() );
   }
#else

   Status = PdhAddCounter(m_cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &m_cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddCounter failed with status:"; 
      Message << std::hex <<  GetLastError();
      m_InitializeOk = false;
      throw shared::exception::CException ( Message.str() );
   }
#endif

   Status = PdhCollectQueryData(m_cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << std::hex << GetLastError();
      m_InitializeOk = false;
      throw shared::exception::CException ( Message.str() );
   }

   m_InitializeOk = true;
}

CWindowsSystemCPULoad::~CWindowsSystemCPULoad()
{
   PDH_STATUS Status;

   if (m_InitializeOk)
   {
      Status = PdhCloseQuery(m_cpuQuery);

      if (Status != ERROR_SUCCESS) 
      {
         std::stringstream Message; 
         Message << "PdhCloseQuery failed with status:"; 
         Message << std::hex << GetLastError();

         YADOMS_LOG(debug) << Message.str();
      }
   }
}

void CWindowsSystemCPULoad::declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context)
{
   if (m_InitializeOk)
   {
      // Declare associated keywords (= values managed by this device)
      context->declareKeyword(m_device, m_keyword);
   }
}

void CWindowsSystemCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   if (m_InitializeOk)
      context->historizeData(m_device, m_keyword);
}

void CWindowsSystemCPULoad::read()
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

   if (m_InitializeOk)
   {
      Status = PdhCollectQueryData(m_cpuQuery);
      if (Status != ERROR_SUCCESS) 
      {
         std::stringstream Message; 
         Message << "PdhCollectQueryData failed with status:"; 
         Message << std::hex << GetLastError();
         throw shared::exception::CException ( Message.str() );
      }

      Status = PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 | PDH_FMT_NOSCALE, &CounterType, &counterVal);

      if (Status != ERROR_SUCCESS) 
      {
         std::stringstream Message; 
         Message << "PdhGetFormattedCounterValue failed with status:"; 
         Message << std::hex << GetLastError();
         throw shared::exception::CException ( Message.str() );
      }

      m_keyword.set((float)counterVal.doubleValue);
      YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  CPU Load : " << m_keyword.formatValue();
   }
   else
   {
      YADOMS_LOG(trace) << m_device << " is desactivated";
   }
}


