#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

#include <sys/types.h>

#define LINUX_SYSINFO_LOADS_SCALE 65536

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CMemoryLoad::CMemoryLoad(const std::string & device)
   :m_device(device), 
    m_keyword( new yApi::historization::CLoad("MemoryLoad"))
{}

CMemoryLoad::~CMemoryLoad()
{}

void CMemoryLoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
   if (!api->keywordExists( m_device, m_keyword->getKeyword()))
      api->declareKeyword(m_device, *m_keyword, details);
}

void CMemoryLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   BOOST_ASSERT_MSG(!!api, "api must be defined");

   api->historizeData(m_device, *m_keyword);
}

void CMemoryLoad::read()
{
   if (sysinfo (&memInfo)!=0)
   {
      std::stringstream Message; 
      Message << "sysinfo failed !"; 
      throw shared::exception::CException ( Message.str() );
   }

   long long totalVirtualMem = memInfo.totalram;

   totalVirtualMem += memInfo.totalswap;
   totalVirtualMem *= memInfo.mem_unit;

   long long virtualMemUsed = ((memInfo.totalram + memInfo.totalswap) - (memInfo.freeram + memInfo.freeswap))*memInfo.mem_unit;

   std::cout << "Mémoire virtuelle utilisée :" << virtualMemUsed << std::endl;
   std::cout << "Mémoire virtuelle totale   :" << totalVirtualMem << std::endl;

   m_keyword->set( virtualMemUsed*100/double(totalVirtualMem));

   std::cout << "Memory Load : " << m_keyword->formatValue() << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CMemoryLoad::GetHistorizable() const
{
	return m_keyword;
}

