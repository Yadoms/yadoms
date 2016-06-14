#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <sys/sysinfo.h>
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
   struct sysinfo memInfo;
   if (sysinfo (&memInfo)!=0)
   {
      std::stringstream Message; 
      Message << "sysinfo failed !"; 
      throw shared::exception::CException(Message.str());
   }

   long long totalVirtualMem = memInfo.totalram;

   totalVirtualMem += memInfo.totalswap;
   totalVirtualMem *= memInfo.mem_unit;

   long long virtualMemUsed = ((memInfo.totalram + memInfo.totalswap) - (memInfo.freeram + memInfo.freeswap))*memInfo.mem_unit;

   std::cout << "Mémoire virtuelle utilisée :" << virtualMemUsed << std::endl;
   std::cout << "Mémoire virtuelle totale   :" << totalVirtualMem << std::endl;

   m_keyword->set(virtualMemUsed*100/double(totalVirtualMem));

   std::cout << "Memory Load : " << m_keyword->formatValue() << std::endl;
}