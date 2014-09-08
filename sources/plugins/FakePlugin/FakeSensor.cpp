#include "stdafx.h"
#include "FakeSensor.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/StringExtension.h>
#include <shared/Log.h>

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
   context->declareDevice(m_deviceId, getModel(), shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceId, "temp1", yApi::CStandardCapacities::Temperature);
   context->declareKeyword(m_deviceId, "temp2", yApi::CStandardCapacities::Temperature);
   context->declareKeyword(m_deviceId, "battery", yApi::CStandardCapacities::BatteryLevel);
   context->declareKeyword(m_deviceId, "rssi", yApi::CStandardCapacities::Rssi);
}

void CFakeSensor::read()
{
   // Generate a random variation on temperature (+/- 0 to 1.0°)
   double offset = (int)(m_dist(m_gen) - 10.0) / 10.0; // Random offset, value from -1.0 to 1.0
   m_temperature1 += offset;

   //we keep 2 decimals
   m_temperature1 = (int)(m_temperature1 * 100) / 100.0;

   // Generate a random variation on temperature (+/- 0 to 2.0°)
   offset = (int)(m_dist(m_gen) - 20.0) / 20.0; // Random offset, value from -2.0 to 2.0
   m_temperature2 += offset;

   //we keep 2 decimals
   m_temperature2 = (int)(m_temperature2 * 100) / 100.0;

   // Decrease battery level (min 20%)
   if (m_batteryLevel > 20)
      m_batteryLevel -= 1;
}

void CFakeSensor::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_deviceId, "temp1", m_temperature1, 1);
   context->historizeData(m_deviceId, "temp2", m_temperature2, 1);
   context->historizeData(m_deviceId, "battery", m_batteryLevel);
   context->historizeData(m_deviceId, "rssi"   , m_rssi        );

   //removed log to display only bad tests on console
   //YADOMS_LOG(debug) << "historize " << m_deviceId << ", temp1=" << m_temperature1 << ", temp2=" << m_temperature2 << ", battery=" << m_batteryLevel << ", Rssi=" << m_rssi;
}

const std::string& CFakeSensor::getModel()
{
   static const std::string model("Fake sensor");
   return model;
}