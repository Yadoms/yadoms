#include "stdafx.h"
#include "Barometric.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{

CBarometric::CBarometric(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName)
   :m_pressure("pressure")
{
   m_PluginName = PluginName;

   	if (!context->keywordExists( m_PluginName, m_pressure.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CBarometric::~CBarometric()
{
}

void CBarometric::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_pressure);
}

void CBarometric::GetValue(double temp)
{
   m_pressure.set(temp);
}

void CBarometric::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_pressure);
}
} // namespace WUCapacities