#include "stdafx.h"
#include "Wind.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWind::CWind(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string Prefix):
   m_PluginName ( PluginName ),
   m_windDirection(Prefix + "WindDirection"),
   m_windAverageSpeed(Prefix + "windAverageSpeed"),
   m_windMaxSpeed (Prefix + "windMaxSpeed")
{
	if (!context->keywordExists( m_PluginName, m_windDirection.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CWind::~CWind()
{}

void CWind::SetValue(const shared::CDataContainer & ValueContainer, const std::string & filterWindDirection, const std::string & filterWindAverageSpeed, const std::string & filterWindMaxSpeed)
{
	m_windDirection.set((int) ValueContainer.get<double>( filterWindDirection ));

	YADOMS_LOG(debug) << m_windDirection.getKeyword() << "=" << m_windDirection.get() << "°";

	// Following Units are in km/h. We have to x 1000 to get meters en divide by 3600 to obtain secondes.
	// The units into the base are in m/s
	m_windAverageSpeed.set(ValueContainer.get<double>( filterWindAverageSpeed )*1000 / 3600);

	YADOMS_LOG(debug) << m_windAverageSpeed.getKeyword() << "=" << m_windAverageSpeed.get() << "m/s";

	m_windMaxSpeed.set(ValueContainer.get<double>( filterWindMaxSpeed ) *1000 / 3600);

	YADOMS_LOG(debug) << m_windMaxSpeed.getKeyword() << "=" << m_windMaxSpeed.get() << "m/s";
}

void CWind::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_windDirection);
	context->declareKeyword(m_PluginName, m_windAverageSpeed);
	context->declareKeyword(m_PluginName, m_windMaxSpeed);
}

void CWind::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_windDirection);
   context->historizeData(m_PluginName, m_windAverageSpeed);
   context->historizeData(m_PluginName, m_windMaxSpeed);
}
