#include "stdafx.h"
#include "Barometric.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CBarometric::CBarometric(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName)
   :m_pressure( KeyWordName )
{
   m_PluginName = PluginName;

   	if (!context->keywordExists( m_PluginName, m_pressure.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CBarometric::~CBarometric()
{}

void CBarometric::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_pressure);
}

void CBarometric::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_pressure.set(ValueContainer.get<double>( filter ));
}

void CBarometric::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_pressure);
}