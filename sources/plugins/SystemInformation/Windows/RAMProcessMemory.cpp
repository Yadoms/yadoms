#include "stdafx.h"
#include "RAMProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <windows.h>
#include <psapi.h>

CRAMProcessMemory::CRAMProcessMemory(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CKByte>(keywordName))
{
}

CRAMProcessMemory::~CRAMProcessMemory()
{
}

void CRAMProcessMemory::read()
{
   PROCESS_MEMORY_COUNTERS pmc;

   if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
   {
      std::stringstream Message;
      Message << "Fail to read Windows process memory size in RAM :";
      Message << GetLastError();
      throw shared::exception::CException(Message.str());
   }

   long RAMProcessMemory = pmc.WorkingSetSize / 1000;

   m_keyword->set(RAMProcessMemory);

   std::cout << "RAM Memory Current Process : " << m_keyword->formatValue() << std::endl;
}

