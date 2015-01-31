#include "stdafx.h"
#include "Rain.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRain::CRain( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_rain( KeyWordName )
{}

void CRain::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_rain.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CRain::~CRain()
{
}

void CRain::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_rain.set(ValueContainer.get<double>( filter ));

	YADOMS_LOG(debug) << m_rain.getKeyword() << "=" << m_rain.get() << "mm";
}

void CRain::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_rain);
}

void CRain::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_rain);
}
