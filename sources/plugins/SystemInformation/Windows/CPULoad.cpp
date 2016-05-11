#include "stdafx.h"
#include "CPULoad.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <pdh.h>
#include <pdhmsg.h>


#pragma comment(lib, "pdh.lib")

//Error Messages could be obtained with this function:
// http://msdn.microsoft.com/en-us/library/aa373046%28VS.85%29.aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724451(v=vs.85).aspx

CCPULoad::CCPULoad(const std::string & device)
   :m_device(device), 
    m_keyword( new yApi::historization::CLoad("CPULoad") ), 
    m_InitializeOk(false)
{
   try
   {
      Initialize();
   }
   catch (shared::exception::CException& e)
	{
      YADOMS_LOG(error) << "Error initializing CPULoad Keyword : "<< m_keyword->getKeyword() << "Error :" << e.what();
	}
}

void CCPULoad::Initialize()
{
   PDH_STATUS Status;
   char ProcessorTimeString[256];
   CHAR ProcessorObjectName[256];
   CHAR CounterPath[256];
   
   DWORD nsize = sizeof(ProcessorTimeString);

   // Create the Query
   Status = PdhOpenQuery(NULL, NULL, &m_cpuQuery);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhOpenQuery failed with status:"; 
      Message << std::hex << Status;
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhLookupPerfNameByIndex( NULL,0x06,ProcessorTimeString, &nsize); // 

   if (Status != ERROR_SUCCESS) 
   {
	  YADOMS_LOG(debug) << "ProcessorTimeString: " << ProcessorTimeString;
      std::stringstream Message; 
      Message << "PdhLookupPerfNameByIndex failed with status:"; 
      Message << std::hex <<  Status;
      throw shared::exception::CException ( Message.str() );
   }

   nsize = sizeof(ProcessorObjectName);
   Status = PdhLookupPerfNameByIndex(NULL, 238, ProcessorObjectName, &nsize);  // Processus object

   if (Status != ERROR_SUCCESS) 
   {
	  YADOMS_LOG(debug) << "ProcessorObjectName: " << ProcessorObjectName;
      std::stringstream Message; 
      Message << "PdhLookupPerfNameByIndex failed with status:"; 
      Message << std::hex <<  Status;
      throw shared::exception::CException ( Message.str() );
   }

   PDH_COUNTER_PATH_ELEMENTS pcpe = {0};
   pcpe.szObjectName = ProcessorObjectName;
   pcpe.szInstanceName = "_Total"; // This parameter is identical for all languages

   pcpe.szCounterName = ProcessorTimeString;
   nsize = sizeof( CounterPath );

   // Create the path of the counter
   Status = PdhMakeCounterPath(&pcpe, CounterPath, &nsize, 0);

   if (Status != ERROR_SUCCESS) 
   {
	  YADOMS_LOG(debug) << "CounterPath: " << CounterPath;
      std::stringstream Message; 
      Message << "PdhMakeCounterPath failed with status:"; 
      Message << std::hex <<  Status;
      throw shared::exception::CException ( Message.str() );
   }

   // Add the selected counter to the query
   Status = PdhAddCounter(m_cpuQuery, TEXT(CounterPath) , NULL, &m_cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddCounter failed with status:"; 
      Message << std::hex <<  Status;
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhCollectQueryData(m_cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << std::hex << Status;
      throw shared::exception::CException ( Message.str() );
   }

   m_InitializeOk = true;
}

CCPULoad::~CCPULoad()
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

void CCPULoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer details)
{
   if (m_InitializeOk)
   {
      if (!context->keywordExists( m_device, m_keyword->getKeyword()))
         context->declareKeyword(m_device, *m_keyword, details);
   }
}

void CCPULoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   if (!context)
      throw shared::exception::CException("context must be defined");

   if (m_InitializeOk)
      context->historizeData(m_device, *m_keyword);
}

void CCPULoad::read()
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

	  float CPULoad = (float) floor(counterVal.doubleValue*10 + 0.5) /10;

      m_keyword->set( CPULoad );

      YADOMS_LOG(debug) << "CPU Load : " << m_keyword->formatValue();
   }
   else
   {
      YADOMS_LOG(trace) << m_device << " is desactivated";
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CCPULoad::GetHistorizable() const
{
	return m_keyword;
}
