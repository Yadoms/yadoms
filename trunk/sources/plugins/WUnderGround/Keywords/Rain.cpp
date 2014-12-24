#include "stdafx.h"
#include "Rain.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{

CRain::CRain(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName)
   :m_rain( KeyWordName )
{
	m_PluginName = PluginName;

	if (!context->keywordExists( m_PluginName, m_rain.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CRain::~CRain()
{
}

void CRain::GetValue(double temp)
{
   m_rain.set(temp);
}

void CRain::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_rain);
}

void CRain::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_rain);
}
} // namespace WUCapacities