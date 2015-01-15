#include "stdafx.h"
#include "Temp.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CTemp::CTemp(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName ,std::string KeyWordName)
   :m_PluginName ( PluginName ), m_temperature( KeyWordName )
{
	if (!context->keywordExists( m_PluginName, m_temperature.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CTemp::~CTemp()
{}

void CTemp::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_temperature);
}

void CTemp::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_temperature.set(ValueContainer.get<double>( filter ));

	YADOMS_LOG(debug) << m_temperature.getKeyword() << "=" << m_temperature.get() << "°C";
}

void CTemp::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_temperature);
}
