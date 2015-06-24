#include "stdafx.h"
#include "UV.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CUV::CUV( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_uv( KeyWordName )
{}


void CUV::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
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

	YADOMS_LOG(debug) << m_uv.getKeyword() << "=" << m_uv.get();
}

void CUV::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_uv);
}

void CUV::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_uv);
}
