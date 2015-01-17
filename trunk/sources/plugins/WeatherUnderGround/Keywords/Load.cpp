#include "stdafx.h"
#include "Load.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CLoad::CLoad(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName)
   :m_PluginName ( PluginName ), m_pourcentage( KeyWordName )
{
   	if (!context->keywordExists( m_PluginName, m_pourcentage.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CLoad::~CLoad()
{}

void CLoad::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_pourcentage);
}

void CLoad::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_pourcentage.set(ValueContainer.get<double>( filter ));

	YADOMS_LOG(debug) << m_pourcentage.getKeyword() << "=" << m_pourcentage.get() << "%";
}

void CLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_pourcentage);
}