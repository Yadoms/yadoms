#include "stdafx.h"
#include "ApparentPowerMeter.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CApparentPowerMeter::CApparentPowerMeter( boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_apparentpower( new yApi::historization::CApparentPower(KeyWordName) )
{
   if (!context->keywordExists( m_PluginName, m_apparentpower->getKeyword()))
	{
		context->declareKeyword( m_PluginName, *m_apparentpower );
	}
}

void CApparentPowerMeter::SetValue( double Value )
{
   m_apparentpower->set( Value );
   YADOMS_LOG(debug) << m_apparentpower->getKeyword() << "=" << m_apparentpower->get() << m_apparentpower->getCapacity().getUnit();
}

CApparentPowerMeter::~CApparentPowerMeter()
{}

boost::shared_ptr<yApi::historization::IHistorizable> CApparentPowerMeter::GetHistorizable() const
{
	return m_apparentpower;
}