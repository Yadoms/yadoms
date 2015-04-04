#include "stdafx.h"
#include "Moon.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <boost/lexical_cast.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CMoon::CMoon( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_moonCharacteristics( new yApi::historization::CMoonHistorizer(KeyWordName, yApi::EKeywordAccessMode::kGetSet ) )
{}

void CMoon::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_moonCharacteristics->getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CMoon::~CMoon()
{}

void CMoon::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, *m_moonCharacteristics);
}

void CMoon::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_moonCharacteristics->AddUnit ( UnitName, UnitValue );
}

void CMoon::SetParameters(
            const shared::CDataContainer & ValueContainer,
            const std::string& filterIlluminatedMoon,
            const std::string& filterDayofMoon
            )
{
	m_moonCharacteristics->SetParameters(
                     ValueContainer.get<std::string>( filterIlluminatedMoon ),
                     ValueContainer.get<std::string>( filterDayofMoon ));
}

boost::shared_ptr<yApi::historization::IHistorizable> CMoon::GetHistorizable() const
{
	return m_moonCharacteristics;
}