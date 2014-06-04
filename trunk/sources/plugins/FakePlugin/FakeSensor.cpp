#include "stdafx.h"
#include "FakeSensor.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>


CFakeSensor::CFakeSensor(const std::string & deviceId)
   :m_deviceId(deviceId), m_temperature1(25.0), m_temperature2(10.0), m_batteryLevel(100), m_rssi(50), m_dist(0, 20)
{
}

CFakeSensor::~CFakeSensor()
{
}

void CFakeSensor::read()
{
   // Generate a random variation on temperature (+/- 0 to 1.0°)
   double offset = (m_dist(m_gen) - 10.0) / 10.0; // Random offset, value from -1.0 to 1.0
   m_temperature1 += offset;

   // Generate a random variation on temperature (+/- 0 to 2.0°)
   offset = (m_dist(m_gen) - 20.0) / 20.0; // Random offset, value from -2.0 to 2.0
   m_temperature2 += offset;

   // Decrease battery level (min 20%)
   if (m_batteryLevel > 20)
      m_batteryLevel -= 1;
}

void CFakeSensor::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(getDeviceId(), "temp1", boost::lexical_cast<std::string>(m_temperature1));//TODO si on pouvait éviter les cast
   context->historizeData(getDeviceId(), "temp2", boost::lexical_cast<std::string>(m_temperature2));
   context->historizeData(getDeviceId(), "battery", boost::lexical_cast<std::string>(m_batteryLevel));
   context->historizeData(getDeviceId(), "Rssi", boost::lexical_cast<std::string>(m_rssi));
}


const std::string& CFakeSensor::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CFakeSensor::getModel()
{
   static const std::string model("Fake sensor");
   return model;
}