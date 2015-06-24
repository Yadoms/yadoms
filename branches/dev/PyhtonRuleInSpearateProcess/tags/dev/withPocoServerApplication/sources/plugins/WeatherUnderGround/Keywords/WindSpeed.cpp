#include "stdafx.h"
#include "WindSpeed.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWindSpeed::CWindSpeed( std::string PluginName, std::string Prefix ):
   m_PluginName ( PluginName ),
   m_windSpeed( new yApi::historization::CSpeed(Prefix + "windAverageSpeed"))
{}

void CWindSpeed::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_windSpeed->getKeyword()))
	{
      context->declareKeyword(m_PluginName, *m_windSpeed);
	}
}

CWindSpeed::~CWindSpeed()
{}

void CWindSpeed::SetValue(const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	// Following Units are in km/h. We have to x 1000 to get meters en divide by 3600 to obtain secondes.
	// The units into the base are in m/s
	m_windSpeed->set(ValueContainer.get<double>( filter )*1000 / 3600);
	YADOMS_LOG(debug) << m_windSpeed->getKeyword() << "=" << m_windSpeed->get() << "m/s";
}

void CWindSpeed::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{}

boost::shared_ptr<yApi::historization::IHistorizable> CWindSpeed::GetHistorizable() const
{
	return m_windSpeed;
}