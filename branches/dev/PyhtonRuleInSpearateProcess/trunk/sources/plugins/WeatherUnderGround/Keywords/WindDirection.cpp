#include "stdafx.h"
#include "WindDirection.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWindDirection::CWindDirection( std::string PluginName, std::string Name ):
   m_PluginName ( PluginName ),
   m_windDirection( new yApi::historization::CDirection( Name ))
{}

void CWindDirection::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_windDirection->getKeyword()))
	{
	   context->declareKeyword(m_PluginName,*m_windDirection);
	}
}

CWindDirection::~CWindDirection()
{}

void CWindDirection::SetValue(const shared::CDataContainer & ValueContainer, const std::string & filterWindDirection)
{
	m_windDirection->set((int) ValueContainer.get<double>( filterWindDirection ));

	YADOMS_LOG(debug) << m_windDirection->getKeyword() << "=" << m_windDirection->get() << "°";
}

void CWindDirection::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{}

boost::shared_ptr<yApi::historization::IHistorizable> CWindDirection::GetHistorizable() const
{
	return m_windDirection;
}
