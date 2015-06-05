#include "stdafx.h"
#include "WeatherIcon.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include "KeywordException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWeatherIcon::CWeatherIcon( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_weathercondition( new yApi::historization::CWeatherCondition(KeyWordName) )
{}

void CWeatherIcon::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_weathercondition->getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CWeatherIcon::~CWeatherIcon()
{
}

void CWeatherIcon::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
    try
	{
        weatherunderground::helper::EnumValuesNames::const_iterator it = weatherunderground::helper::EEnumTypeNames.find( ValueContainer.get<std::string>( filter ) );
        if (it != weatherunderground::helper::EEnumTypeNames.end())
	    {
           m_weathercondition->set( (yApi::historization::EWeatherCondition)(it->second) );

		   YADOMS_LOG(debug) << m_weathercondition->getKeyword() << "=" << m_weathercondition->get();
	    }
	    else
		   throw CKeywordException ("Keyword WeatherIcon could not be set");
	}
    catch (shared::exception::CException e)
	{
		YADOMS_LOG(warning) << e.what() << std::endl;
	}
}

void CWeatherIcon::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, *m_weathercondition);
}

boost::shared_ptr<yApi::historization::IHistorizable> CWeatherIcon::GetHistorizable() const
{
	return m_weathercondition;
}