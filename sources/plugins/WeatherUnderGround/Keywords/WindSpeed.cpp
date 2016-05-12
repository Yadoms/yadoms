#include "stdafx.h"
#include "WindSpeed.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWindSpeed::CWindSpeed(std::string PluginName,
                       std::string Name):
   m_PluginName(PluginName),
   m_windSpeed(boost::make_shared<yApi::historization::CSpeed>(Name))
{
}

void CWindSpeed::Initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                            shared::CDataContainer details) const
{
   if (!api->keywordExists(m_PluginName, m_windSpeed->getKeyword()))
      api->declareKeyword(m_PluginName, *m_windSpeed, details);
}

CWindSpeed::~CWindSpeed()
{
}

void CWindSpeed::SetValue(const shared::CDataContainer& ValueContainer,
                          const std::string& filter) const
{
   // Following Units are in km/h. We have to x 1000 to get meters en divide by 3600 to obtain secondes.
   // The units into the base are in m/s
   m_windSpeed->set(ValueContainer.get<double>(filter) * 1000 / 3600);
   std::cout << m_windSpeed->getKeyword() << "=" << m_windSpeed->get() << "m/s" << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CWindSpeed::GetHistorizable() const
{
   return m_windSpeed;
}

