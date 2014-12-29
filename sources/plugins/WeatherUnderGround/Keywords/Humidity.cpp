#include "stdafx.h"
#include "Humidity.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CHumidity::CHumidity(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName)
   :m_humidity( KeyWordName )
{
	m_PluginName = PluginName;

	if (!context->keywordExists( m_PluginName, m_humidity.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CHumidity::~CHumidity()
{
}

void CHumidity::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	std::string str_humidity = ValueContainer.get<std::string>( filter );
	str_humidity.pop_back();
	double d_humidity = (double)atof(str_humidity.c_str());

	m_humidity.set( d_humidity );
}

void CHumidity::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_humidity);
}

void CHumidity::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_humidity);
}