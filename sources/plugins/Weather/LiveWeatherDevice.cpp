#include "stdafx.h"
#include "LiveWeatherDevice.h"


CLiveWeatherDevice::CLiveWeatherDevice(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     /*TODO ajouter autres KW*/
     m_keywords({m_temperature})

{
}

void CLiveWeatherDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api->deviceExists(m_deviceName))
   {
      api->declareDevice(m_deviceName,
                         m_deviceName,
                         m_deviceName,
                         m_keywords);
   }
}
