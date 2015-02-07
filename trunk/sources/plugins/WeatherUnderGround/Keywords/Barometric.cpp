#include "stdafx.h"
#include "Barometric.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CBarometric::CBarometric( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_pressure( new yApi::historization::CPressure(KeyWordName) )
{}

void CBarometric::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_pressure->getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CBarometric::~CBarometric()
{}

void CBarometric::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, *m_pressure);
}

void CBarometric::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_pressure->set(ValueContainer.get<double>( filter ));

	YADOMS_LOG(debug) << m_pressure->getKeyword() << "=" << m_pressure->get() << "mbar";
}

boost::shared_ptr<yApi::historization::IHistorizable> CBarometric::GetHistorizable() const
{
	return m_pressure;
}
