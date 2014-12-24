#include "stdafx.h"
#include "Visibility.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{

CVisibility::CVisibility(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName)
   :m_visibility("Visibility")
{
	m_PluginName = PluginName;

	if (!context->keywordExists( m_PluginName, m_visibility.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CVisibility::~CVisibility()
{
}

void CVisibility::GetValue(double temp)
{
   m_visibility.set(temp);
}

void CVisibility::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_visibility);
}

void CVisibility::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_visibility);
}
} // namespace WUCapacities