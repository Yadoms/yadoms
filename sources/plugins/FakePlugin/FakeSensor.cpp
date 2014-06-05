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

void CFakeSensor::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, getModel());

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceId, "temp1"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   context->declareKeyword(m_deviceId, "temp2"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   context->declareKeyword(m_deviceId, "battery", yApi::CStandardCapacities::BatteryLevel, yApi::IYadomsApi::kReadOnly);
   context->declareKeyword(m_deviceId, "Rssi"   , yApi::CStandardCapacities::Rssi        , yApi::IYadomsApi::kReadOnly);
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
   context->historizeData(m_deviceId, "temp1", boost::lexical_cast<std::string>(m_temperature1));//TODO si on pouvait éviter les cast
   context->historizeData(m_deviceId, "temp2", boost::lexical_cast<std::string>(m_temperature2));
   context->historizeData(m_deviceId, "battery", boost::lexical_cast<std::string>(m_batteryLevel));
   context->historizeData(m_deviceId, "Rssi", boost::lexical_cast<std::string>(m_rssi));
}

const std::string& CFakeSensor::getModel()
{
   static const std::string model("Fake sensor");
   return model;
}