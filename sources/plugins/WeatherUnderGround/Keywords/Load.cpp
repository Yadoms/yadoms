#include "stdafx.h"
#include "Load.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CLoad::CLoad( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_pourcentage( new yApi::historization::CLoad(KeyWordName) )
{}


void CLoad::Initialize( boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer details ) const
{
   if (!context->keywordExists( m_PluginName, m_pourcentage->getKeyword()))
	{
      context->declareKeyword(m_PluginName, *m_pourcentage, details);
	}
}

CLoad::~CLoad()
{}

void CLoad::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_pourcentage->set(ValueContainer.get<double>( filter ));

	YADOMS_LOG(debug) << m_pourcentage->getKeyword() << "=" << m_pourcentage->get() << "%";
}

boost::shared_ptr<yApi::historization::IHistorizable> CLoad::GetHistorizable() const
{
	return m_pourcentage;
}