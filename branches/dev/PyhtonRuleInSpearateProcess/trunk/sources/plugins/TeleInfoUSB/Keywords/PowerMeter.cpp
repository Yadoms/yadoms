#include "stdafx.h"
#include "PowerMeter.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CPowerMeter::CPowerMeter( boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_energy( new yApi::historization::CEnergy(KeyWordName) )
{
   if (!context->keywordExists( m_PluginName, m_energy->getKeyword()))
	{
      context->declareKeyword ( m_PluginName, *m_energy );
	}
}

void CPowerMeter::SetValue( long Value )
{
   m_energy->set( Value );
   YADOMS_LOG(debug) << m_energy->getKeyword() << "=" << m_energy->get() << "W";
}

CPowerMeter::~CPowerMeter()
{}

boost::shared_ptr<yApi::historization::IHistorizable> CPowerMeter::GetHistorizable() const
{
	return m_energy;
}