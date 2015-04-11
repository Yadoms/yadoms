#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

#include <sys/types.h>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CMemoryLoad::CMemoryLoad(const std::string & device)
   :m_device(device), 
    m_keyword( new yApi::historization::CLoad("MemoryLoad"))
{}

CMemoryLoad::~CMemoryLoad()
{}

void CMemoryLoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (!context->keywordExists( m_device, m_keyword->getKeyword()))
   {
      context->declareKeyword(m_device, *m_keyword);
   }
}

void CMemoryLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
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

   long long virtualMemUsed = memInfo.totalram - memInfo.freeram;

   YADOMS_LOG(debug) << "Mémoire virtuelle utilisée :" << virtualMemUsed;
   YADOMS_LOG(debug) << "Mémoire virtuelle totale   :" << totalVirtualMem;

   //TODO: Cette méthode renvoie une valeur supérieure à ce que me renvoie le moniteur système d'Ubuntu ... A vérifier. Domoticz donne la meme chose. A vérifier avec une autre fonction mémoire en ligne de commande.

   m_keyword->set( virtualMemUsed*100/double(totalVirtualMem));

   YADOMS_LOG(debug) << "Memory Load : " << m_keyword->formatValue();
}

boost::shared_ptr<yApi::historization::IHistorizable> CMemoryLoad::GetHistorizable() const
{
	return m_keyword;
}

