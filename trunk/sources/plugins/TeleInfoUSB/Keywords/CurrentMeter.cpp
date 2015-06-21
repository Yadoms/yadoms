#include "stdafx.h"
#include "CurrentMeter.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CCurrentMeter::CCurrentMeter( boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_current( new yApi::historization::CCurrent(KeyWordName) )
{
   if (!context->keywordExists( m_PluginName, m_current->getKeyword()))
	{
      context->declareKeyword( m_PluginName, *m_current );
	}
}

void CCurrentMeter::SetValue( double Value )
{
   m_current->set( Value );
   YADOMS_LOG(debug) << m_current->getKeyword() << "=" << m_current->get() << "A";
}

CCurrentMeter::~CCurrentMeter()
{}

boost::shared_ptr<yApi::historization::IHistorizable> CCurrentMeter::GetHistorizable() const
{
	return m_current;
}