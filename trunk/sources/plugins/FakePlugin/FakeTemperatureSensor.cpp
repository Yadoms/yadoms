#include "stdafx.h"
#include "FakeTemperatureSensor.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>


CFakeTemperatureSensor::CFakeTemperatureSensor(const std::string & deviceId)
   :m_deviceId(deviceId), m_temperature1(25.0), m_temperature2(10.0), m_batteryLevel(100), m_rssi(50), m_dist(0, 20)
{
}

CFakeTemperatureSensor::~CFakeTemperatureSensor()
{
}

void CFakeTemperatureSensor::read()
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

const std::string& CFakeTemperatureSensor::getDeviceId() const
{
   return m_deviceId;
}

double CFakeTemperatureSensor::getTemperature1() const
{
   return m_temperature1;
}

double CFakeTemperatureSensor::getTemperature2() const
{
   return m_temperature2;
}

int CFakeTemperatureSensor::getBatteryLevel() const
{
   return m_batteryLevel;
}

int CFakeTemperatureSensor::getRssi() const
{
   return m_rssi;
}

const std::vector<yApi::CCapacity>& CFakeTemperatureSensor::getCapacities()
{
   static const std::vector<yApi::CCapacity> capacities = boost::assign::list_of
      (yApi::CStandardCapacities::getTemperatureSensorCapacity())
      (yApi::CStandardCapacities::getBatteryLevelCapacity())
      (yApi::CStandardCapacities::getRssiMeasureCapacity());

   return capacities;
}