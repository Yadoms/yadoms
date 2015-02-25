#include "stdafx.h"
#include "FakeSensor.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/StringExtension.h>
#include <shared/Log.h>

CFakeSensor::CFakeSensor(const std::string& deviceName)
   :m_deviceName(deviceName), m_temperature1("temp1"), m_temperature2("temp2"), m_batteryLevel("Battery"), 
   m_current("current", yApi::EKeywordAccessMode::kGet, yApi::historization::EMeasureType::kAbsolute, yApi::historization::typeInfo::CDoubleTypeInfo().setMin(0).setMax(5).setPrecision(0.1)),
   m_rssi("rssi"), m_dateTime("dateTime", shared::plugin::yPluginApi::EKeywordAccessMode::kGet), m_dist(0, 20)
{
   m_temperature1.set(25.0);
   m_temperature2.set(10.0);
   m_batteryLevel.set(100);
   m_current.set(2);
   m_rssi.set(50);
   m_dateTime.set(boost::posix_time::second_clock::local_time());
}

CFakeSensor::~CFakeSensor()
{
}

void CFakeSensor::declareDevice(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (!context->deviceExists(m_deviceName))
      context->declareDevice(m_deviceName, getModel());

   // Declare associated keywords (= values managed by this device)
   if (!context->keywordExists(m_deviceName, m_temperature1))
      context->declareKeyword(m_deviceName, m_temperature1);
   if (!context->keywordExists(m_deviceName, m_temperature2))
      context->declareKeyword(m_deviceName, m_temperature2);
   if (!context->keywordExists(m_deviceName, m_current))
      context->declareKeyword(m_deviceName, m_current);
   if (!context->keywordExists(m_deviceName, m_batteryLevel))
      context->declareKeyword(m_deviceName, m_batteryLevel);
   if (!context->keywordExists(m_deviceName, m_rssi))
      context->declareKeyword(m_deviceName, m_rssi);
   if (!context->keywordExists(m_deviceName, m_dateTime))
      context->declareKeyword(m_deviceName, m_dateTime);
}

void CFakeSensor::read()
{
   // Generate a random variation on temperature (+/- 0 to 1.0°)
   double offset = static_cast<int>(m_dist(m_gen) - 10.0) / 10.0; // Random offset, value from -1.0 to 1.0
   double temperature = m_temperature1() + offset;

   //we keep 2 decimals
   m_temperature1.set(static_cast<int>(temperature * 100) / 100.0);

   // Generate a random variation on temperature (+/- 0 to 2.0°)
   offset = static_cast<int>(m_dist(m_gen) - 20.0) / 20.0; // Random offset, value from -2.0 to 2.0
   temperature = m_temperature2() + offset;

   //we keep 2 decimals
   m_temperature2.set(static_cast<int>(temperature * 100) / 100.0);

   // Decrease battery level (min 20%)
   if (m_batteryLevel() > 20)
      m_batteryLevel.set(m_batteryLevel() - 1);

   //set the current date time onto m_datetime keyword
   m_dateTime.set(boost::posix_time::second_clock::local_time());

   // Generate a random variation on temperature (+/- 0 to 1°)
   offset = static_cast<int>(m_dist(m_gen) - 10.0) / 10.0; // Random offset, value from -1.0 to 1.0
   double current = m_current() + offset;
   if (current < 0) 
      current = 0;
   if (current > 5)
      current = 5;
   m_current.set(static_cast<int>(current * 10) / 10.0);
}

void CFakeSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_deviceName, m_temperature1);
   context->historizeData(m_deviceName, m_temperature2);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi        );
   context->historizeData(m_deviceName, m_dateTime    );
}

const std::string& CFakeSensor::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeSensor::getModel()
{
   static const std::string model("Fake sensor");
   return model;
}
