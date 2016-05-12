#include "stdafx.h"
#include "Rain.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRain::CRain(std::string PluginName,
             std::string KeyWordName)
   : m_PluginName(PluginName),
     m_rain(new yApi::historization::CRain(KeyWordName))
{
}

void CRain::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details) const
{
   if (!api->keywordExists(m_PluginName, m_rain->getKeyword()))
   {
      api->declareKeyword(m_PluginName, *m_rain, details);
   }
}

CRain::~CRain()
{
}

void CRain::SetValue(const shared::CDataContainer& ValueContainer,
                     const std::string& filter) const
{
   if (ValueContainer.get<std::string>(filter) == "--")
   {
      std::cout << m_rain->getKeyword() << " : '--' => Value not registered" << std::endl;
   }
   else
   {
      m_rain->set(ValueContainer.get<double>(filter));
      std::cout << m_rain->getKeyword() << "=" << m_rain->get() << "mm" << std::endl;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CRain::GetHistorizable() const
{
   return m_rain;
}

