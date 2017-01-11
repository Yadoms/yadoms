#include "stdafx.h"
#include "WindSpeed.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWindSpeed::CWindSpeed(const std::string& pluginName,
                       const std::string& name):
   m_pluginName(pluginName),
   m_windSpeed(boost::make_shared<yApi::historization::CSpeed>(name))
{
}

CWindSpeed::~CWindSpeed()
{
}

void CWindSpeed::setValue(const shared::CDataContainer& valueContainer,
                          const std::string& filter) const
{
   // Following Units are in km/h. We have to x 1000 to get meters en divide by 3600 to obtain secondes.
   // The units into the base are in m/s
   m_windSpeed->set(valueContainer.get<double>(filter) * 1000 / 3600);
   std::cout << m_windSpeed->getKeyword() << "=" << m_windSpeed->get() << "m/s" << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CWindSpeed::getHistorizable() const
{
   return m_windSpeed;
}

