#include "stdafx.h"
#include "UV.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{

CUV::CUV(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName)
   :m_uv("UV")
{
	m_PluginName = PluginName;

	if (!context->keywordExists( m_PluginName, m_uv.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CUV::~CUV()
{
}

void CUV::GetValue(double temp)
{
   m_uv.set(temp);
}

void CUV::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_uv);
}

void CUV::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_uv);
}
} // namespace WUCapacities