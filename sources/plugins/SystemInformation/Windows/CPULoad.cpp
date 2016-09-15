#include "stdafx.h"
#include "CPULoad.h"
#include <shared/exception/Exception.hpp>
#include <pdh.h>
#include <pdhmsg.h>


#pragma comment(lib, "pdh.lib")

//Error Messages could be obtained with this function:
// http://msdn.microsoft.com/en-us/library/aa373046%28VS.85%29.aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724451(v=vs.85).aspx

CCPULoad::CCPULoad(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName)),
   m_InitializeOk(false)
{
   try
   {
      Initialize();
   }
   catch (shared::exception::CException& e)
   {
      std::cerr << "Error initializing CPULoad Keyword : " << m_keyword->getKeyword() << "Error :" << e.what() << std::endl;
   }
}

void CCPULoad::Initialize()
{
   PDH_STATUS Status;
   char ProcessorTimeString[256];
   CHAR ProcessorObjectName[256];
   char *CounterPath;

   DWORD nsize = sizeof(ProcessorTimeString);

   // Create the Query
   Status = PdhOpenQuery(nullptr, NULL, &m_cpuQuery);

   if (Status != ERROR_SUCCESS)
   {
      std::stringstream Message;
      Message << "PdhOpenQuery failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   Status = PdhLookupPerfNameByIndex(nullptr, 0x06, ProcessorTimeString, &nsize); // 

   if (Status != ERROR_SUCCESS)
   {
      std::cout << "ProcessorTimeString: " << ProcessorTimeString << std::endl;
      std::stringstream Message;
      Message << "PdhLookupPerfNameByIndex failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   nsize = sizeof(ProcessorObjectName);
   Status = PdhLookupPerfNameByIndex(nullptr, 238, ProcessorObjectName, &nsize); // Processus object

   if (Status != ERROR_SUCCESS)
   {
      std::cout << "ProcessorObjectName: " << ProcessorObjectName << std::endl;
      std::stringstream Message;
      Message << "PdhLookupPerfNameByIndex failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   PDH_COUNTER_PATH_ELEMENTS pcpe = { nullptr };
   pcpe.szObjectName = ProcessorObjectName;
   pcpe.szInstanceName = "_Total"; // This parameter is identical for all languages

   pcpe.szCounterName = ProcessorTimeString;

   nsize = 0;

   // Read the size of the counter  path
   Status = PdhMakeCounterPath(&pcpe, NULL, &nsize, 0);

   if (Status != PDH_MORE_DATA)
   {
      std::cout << "Status: " << std::hex << Status << std::endl;
      std::cout << "size: " << nsize << std::endl;
      std::stringstream Message;
      Message << "PdhMakeCounterPath failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   CounterPath = (char*)malloc(nsize);

   // Create the path of the counter
   Status = PdhMakeCounterPath(&pcpe, CounterPath, &nsize, PDH_PATH_WBEM_INPUT ); //0

   if (Status != ERROR_SUCCESS)
   {
      std::cout << "CounterPath: " << CounterPath << std::endl;
      std::stringstream Message;
      Message << "PdhMakeCounterPath failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   // Add the selected counter to the query
   Status = PdhAddCounter(m_cpuQuery, TEXT(CounterPath), NULL, &m_cpuTotal);

   if (Status != ERROR_SUCCESS)
   {
      std::stringstream Message;
      Message << "PdhAddCounter failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   Status = PdhCollectQueryData(m_cpuQuery);
   if (Status != ERROR_SUCCESS)
   {
      std::stringstream Message;
      Message << "PdhCollectQueryData failed with status:";
      Message << std::hex << Status;
      throw shared::exception::CException(Message.str());
   }

   free(CounterPath);

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
         Message << std::hex << Status;
         std::cout << Message.str() << std::endl;
      }
   }
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
         Message << std::hex << Status;
         throw shared::exception::CException(Message.str());
      }

      Status = PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 | PDH_FMT_NOSCALE, &CounterType, &counterVal);

      switch (Status)
      {
         // No Error
      case ERROR_SUCCESS:
      {
         auto CPULoad = static_cast<float>(floor(counterVal.doubleValue * 10 + 0.5)) / 10;
         m_keyword->set(CPULoad);
         std::cout << "CPU Load : " << m_keyword->formatValue() << std::endl;
      }
      break;
      // Negative value ! No Value historize
      case PDH_CALC_NEGATIVE_DENOMINATOR:
      case PDH_CALC_NEGATIVE_VALUE:
         std::cout << "CPU Load : Negative value detected. No Historization." << std::endl;
         break;

         // Undefined error -> throw an exception
      default:
         std::stringstream Message;
         Message << "PdhGetFormattedCounterValue failed with status:";
         Message << std::hex << Status;
         throw shared::exception::CException(Message.str());
         break;
      }
   }
   else
   {
      std::cout << m_keyword->getKeyword() << " is disabled" << std::endl;
   }
}