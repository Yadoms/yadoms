#include "stdafx.h"
#include "VirtualProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <windows.h>
#include <psapi.h>

CVirtualProcessMemory::CVirtualProcessMemory(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CKByte>(keywordName))
{
}

CVirtualProcessMemory::~CVirtualProcessMemory()
{
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

