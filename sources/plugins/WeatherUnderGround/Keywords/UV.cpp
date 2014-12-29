#include "stdafx.h"
#include "UV.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CUV::CUV(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName)
   :m_uv( KeyWordName )
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

void CUV::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_uv.set((int) ValueContainer.get<double>( filter ));
}

void CUV::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_uv);
}

void CUV::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_uv);
}