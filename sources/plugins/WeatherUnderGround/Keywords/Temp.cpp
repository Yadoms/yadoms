#include "stdafx.h"
#include "Temp.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CTemp::CTemp(std::string PluginName,
             std::string KeyWordName)
   : m_PluginName(PluginName),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>(KeyWordName, yApi::EKeywordAccessMode::kGet))
{
}

void CTemp::Initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                       shared::CDataContainer details) const
{
   if (!api->keywordExists(m_PluginName, m_temperature->getKeyword()))
      api->declareKeyword(m_PluginName, *m_temperature, details);
}

CTemp::~CTemp()
{
}

void CTemp::SetValue(const shared::CDataContainer& ValueContainer,
                     const std::string& filter) const
{
   if (ValueContainer.get<std::string>(filter) == "NA")
      std::cout << m_temperature->getKeyword() << " : NA => Value not registered" << std::endl;
   else
   {
      m_temperature->set(ValueContainer.get<double>(filter));
      std::cout << m_temperature->getKeyword() << "=" << m_temperature->get() << " celcius degrees" << std::endl;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CTemp::GetHistorizable() const
{
   return m_temperature;
}

