#include "stdafx.h"
#include "Moon.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CMoon::CMoon(std::string pluginName,
             std::string keyWordName)
   : m_pluginName(pluginName),
     m_moonCharacteristics(boost::make_shared<yApi::historization::CMoonHistorizer>(keyWordName,
                                                                                    yApi::EKeywordAccessMode::kGet))
{
}

void CMoon::initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                       shared::CDataContainer details) const
{
   if (!api->keywordExists(m_pluginName, m_moonCharacteristics))
      api->declareKeyword(m_pluginName, m_moonCharacteristics, details);
}

CMoon::~CMoon()
{
}

void CMoon::addUnit(const std::string& unitName,
                    const std::string& unitValue) const
{
   m_moonCharacteristics->addUnit(unitName,
                                  unitValue);
}

void CMoon::setParameters(const shared::CDataContainer& valueContainer,
                          const std::string& illuminatedMoon,
                          const std::string& dayofMoon) const
{
   m_moonCharacteristics->setParameters(valueContainer.get<std::string>(illuminatedMoon),
                                        valueContainer.get<std::string>(dayofMoon));
}

boost::shared_ptr<yApi::historization::IHistorizable> CMoon::getHistorizable() const
{
   return m_moonCharacteristics;
}

