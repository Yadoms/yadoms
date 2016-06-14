#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>

#include <sys/types.h>

#define LINUX_SYSINFO_LOADS_SCALE 65536

CMemoryLoad::CMemoryLoad(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
}

CMemoryLoad::~CMemoryLoad()
{
}

void CMemoryLoad::read()
{
   if (sysinfo (&m_memInfo)!=0)
   {
      std::stringstream Message; 
      Message << "sysinfo failed !"; 
      throw shared::exception::CException(Message.str());
   }

   long long totalVirtualMem = m_memInfo.totalram;

   totalVirtualMem += m_memInfo.totalswap;
   totalVirtualMem *= m_memInfo.mem_unit;

   long long virtualMemUsed = ((m_memInfo.totalram + m_memInfo.totalswap) - (m_memInfo.freeram + m_memInfo.freeswap))*m_memInfo.mem_unit;

   std::cout << "Mémoire virtuelle utilisée :" << virtualMemUsed << std::endl;
   std::cout << "Mémoire virtuelle totale   :" << totalVirtualMem << std::endl;

   m_keyword->set(virtualMemUsed*100/double(totalVirtualMem));

   std::cout << "Memory Load : " << m_keyword->formatValue() << std::endl;
}