#include "stdafx.h"
#include "VirtualProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <windows.h>
#include <psapi.h>

CVirtualProcessMemory::CVirtualProcessMemory(const std::string& device)
   : m_device(device),
     m_keyword(boost::make_shared<yApi::historization::CKByte>("YadomsVirtualProcessMemory"))
{
}

CVirtualProcessMemory::~CVirtualProcessMemory()
{
}

void CVirtualProcessMemory::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
   if (!api->keywordExists(m_device, m_keyword))
      api->declareKeyword(m_device, m_keyword, details);
}

void CVirtualProcessMemory::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   api->historizeData(m_device, m_keyword);
}

void CVirtualProcessMemory::read()
{
   PROCESS_MEMORY_COUNTERS_EX pmc;

   if (!GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc)))
   {
      std::stringstream Message;
      Message << "Fail to read Windows virtual process memory size :";
      Message << GetLastError();
      throw shared::exception::CException(Message.str());
   }

   long VirtualProcessMemory = pmc.PrivateUsage / 1000;

   m_keyword->set(VirtualProcessMemory);

   std::cout << "Virtual Memory for Current Process : " << m_keyword->formatValue() << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CVirtualProcessMemory::GetHistorizable() const
{
   return m_keyword;
}

