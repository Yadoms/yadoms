#include "stdafx.h"
#include "Temp.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CTemp::CTemp(std::string pluginName,
             std::string keyWordName)
   : m_pluginName(pluginName),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>(keyWordName,
                                                                         yApi::EKeywordAccessMode::kGet))
{
}

CTemp::~CTemp()
{
}

void CTemp::setValue(const shared::CDataContainer& valueContainer,
                     const std::string& filter) const
{
   if (valueContainer.get<std::string>(filter) == "NA")
      std::cout << m_temperature->getKeyword() << " : NA => Value not registered" << std::endl;
   else
   {
      m_temperature->set(valueContainer.get<double>(filter));
      std::cout << m_temperature->getKeyword() << "=" << m_temperature->get() << " celcius degrees" << std::endl;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CTemp::getHistorizable() const
{
   return m_temperature;
}

