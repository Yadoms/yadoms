#include "stdafx.h"
#include "Visibility.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CVisibility::CVisibility( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_visibility( KeyWordName )
{}

void CVisibility::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_visibility.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CVisibility::~CVisibility()
{}

void CVisibility::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	// x 1000 -> The visibility from the web site is in kilometer
	m_visibility.set(ValueContainer.get<double>( filter ) * 1000 );

	YADOMS_LOG(debug) << m_visibility.getKeyword() << "=" << m_visibility.get() << "m";
}

void CVisibility::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_visibility);
}

void CVisibility::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_visibility);
}
