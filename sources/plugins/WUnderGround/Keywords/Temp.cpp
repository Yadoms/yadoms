#include "stdafx.h"
#include "Temp.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{

CTemp::CTemp(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName ,std::string KeyWordName, std::string Unit)
   :m_temperature( KeyWordName )
{
	m_PluginName = PluginName;

	if (!context->keywordExists( m_PluginName, m_temperature.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CTemp::~CTemp()
{}

void CTemp::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	//TODO : Déclarer en utilisant l'unité
	context->declareKeyword(m_PluginName, m_temperature);
}

void CTemp::GetValue(double temp)
{
   m_temperature.set(temp);
}

void CTemp::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_temperature);
}
} // namespace WUCapacities