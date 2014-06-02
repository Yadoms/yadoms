#include "stdafx.h"
#include "FakeTemperatureSensor.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>


CFakeTemperatureSensor::CFakeTemperatureSensor(const std::string & deviceId)
   :m_deviceId(deviceId), m_temperature(25.0), m_batteryLevel(100), m_rssi(50), m_dist(0, 20)
{
}

CFakeTemperatureSensor::~CFakeTemperatureSensor()
{
}

void CFakeTemperatureSensor::read()
{
   // Generate a random variation on temperature (+/- 0 to 1.0°)
   double offset = (m_dist(m_gen) - 10.0) / 10.0; // Radom offset, value from -1.0 to 1.0
   m_temperature += offset;

   // Decrease battery level (min 20%)
   if (m_batteryLevel > 20)
      m_batteryLevel -= 1;
}

const std::string& CFakeTemperatureSensor::getDeviceId() const
{
   return m_deviceId;
}

double CFakeTemperatureSensor::getTemperature() const
{
   return m_temperature;
}

int CFakeTemperatureSensor::getBatteryLevel() const
{
   return m_batteryLevel;
}

int CFakeTemperatureSensor::getRssi() const
{
   return m_rssi;
}

const std::vector<shared::plugin::yadomsApi::CCapacity>& CFakeTemperatureSensor::getCapacities()
{
   static const std::vector<shared::plugin::yadomsApi::CCapacity> capacities = boost::assign::list_of
      (shared::plugin::yadomsApi::CStandardCapacities::getTemperatureSensorCapacity())
      (shared::plugin::yadomsApi::CStandardCapacities::getBatteryLevelCapacity())
      (shared::plugin::yadomsApi::CStandardCapacities::getRssiMeasureCapacity());

   return capacities;
}