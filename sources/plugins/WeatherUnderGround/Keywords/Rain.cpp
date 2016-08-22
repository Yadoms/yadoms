#include "stdafx.h"
#include "Rain.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRain::CRain(std::string pluginName,
             std::string keyWordName)
   : m_pluginName(pluginName),
     m_rain(boost::make_shared<yApi::historization::CRain>(keyWordName))
{
}

CRain::~CRain()
{
}

void CRain::setValue(const shared::CDataContainer& valueContainer,
                     const std::string& filter) const
{
   if (valueContainer.get<std::string>(filter) == "--")
   {
      std::cout << m_rain->getKeyword() << " : '--' => Value not registered" << std::endl;
   }
   else
   {
      m_rain->set(valueContainer.get<double>(filter));
      std::cout << m_rain->getKeyword() << "=" << m_rain->get() << "mm" << std::endl;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CRain::getHistorizable() const
{
   return m_rain;
}

