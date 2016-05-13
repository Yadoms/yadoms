#include "stdafx.h"
#include "RAMProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <windows.h>
#include <psapi.h>

CRAMProcessMemory::CRAMProcessMemory(const std::string & device)
   :m_device(device), 
    m_keyword(new yApi::historization::CKByte("YadomsRAMProcessMemory"))
{}

CRAMProcessMemory::~CRAMProcessMemory()
{}

void CRAMProcessMemory::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
      if (!api->keywordExists( m_device, m_keyword->getKeyword()))
         api->declareKeyword(m_device, *m_keyword, details);
}

void CRAMProcessMemory::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   api->historizeData(m_device, *m_keyword);
}

void CRAMProcessMemory::read()
{
   PROCESS_MEMORY_COUNTERS pmc;

   if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
   {
      std::stringstream Message;
      Message << "Fail to read Windows process memory size in RAM :";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   long RAMProcessMemory = pmc.WorkingSetSize / 1000;
   
   m_keyword->set( RAMProcessMemory );

   std::cout << "RAM Memory Current Process : " << m_keyword->formatValue() << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CRAMProcessMemory::GetHistorizable() const
{
	return m_keyword;
}
