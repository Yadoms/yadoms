#include "stdafx.h"
#include "Moon.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CMoon::CMoon(std::string PluginName,
             std::string KeyWordName)
   : m_PluginName(PluginName),
     m_moonCharacteristics(boost::make_shared<yApi::historization::CMoonHistorizer>(KeyWordName, yApi::EKeywordAccessMode::kGet))
{
}

void CMoon::Initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                       shared::CDataContainer details) const
{
   if (!api->keywordExists(m_PluginName, m_moonCharacteristics->getKeyword()))
      api->declareKeyword(m_PluginName, *m_moonCharacteristics, details);
}

CMoon::~CMoon()
{
}

void CMoon::AddUnit(const std::string& UnitName,
                    const std::string& UnitValue) const
{
   m_moonCharacteristics->AddUnit(UnitName, UnitValue);
}

void CMoon::SetParameters(const shared::CDataContainer& ValueContainer,
                          const std::string& filterIlluminatedMoon,
                          const std::string& filterDayofMoon) const
{
   m_moonCharacteristics->SetParameters(ValueContainer.get<std::string>(filterIlluminatedMoon),
                                        ValueContainer.get<std::string>(filterDayofMoon));
}

boost::shared_ptr<yApi::historization::IHistorizable> CMoon::GetHistorizable() const
{
   return m_moonCharacteristics;
}

