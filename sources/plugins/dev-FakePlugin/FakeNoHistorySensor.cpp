#include "stdafx.h"
#include "FakeNoHistorySensor.h"
#include <shared/tools/Random.h>

CFakeNoHistorySensor::CFakeNoHistorySensor(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature",
                                                                         yApi::EKeywordAccessMode::kGet,
                                                                         yApi::historization::EMeasureType::kAbsolute,
                                                                         yApi::historization::typeInfo::CDoubleTypeInfo::Empty,
                                                                         yApi::historization::EHistoryDepth::kNoHistory)),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity",
                                                                   yApi::EKeywordAccessMode::kGet,
                                                                   yApi::historization::EMeasureType::kAbsolute,
                                                                   yApi::historization::typeInfo::CDoubleTypeInfo::Empty,
                                                                   yApi::historization::EHistoryDepth::kNoHistory)),
     m_incrementalCounter(boost::make_shared<yApi::historization::CCounter>("incrementCount",
                                                                            yApi::EKeywordAccessMode::kGet,
                                                                            yApi::historization::EMeasureType::kIncrement,
                                                                            yApi::historization::typeInfo::CIntTypeInfo::Empty,
                                                                            yApi::historization::EHistoryDepth::kNoHistory)),
     m_historizers({m_temperature, m_humidity, m_incrementalCounter})
{
   m_temperature->set(25.0);
   m_humidity->set(80.0);
   m_incrementalCounter->set(0);
}

void CFakeNoHistorySensor::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   // Declare device and associated keywords (= values managed by this device)
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getType(), getModel(), m_historizers);
}

void CFakeNoHistorySensor::read() const
{
   // Generate a random variation on temperature (-1.0° to 1.0°)
   auto offset = static_cast<int>(shared::tools::CRandom::generate(-10, 10)) / 10.0;
   const auto temperature = m_temperature->get() + offset;

   //we keep 2 decimals
   m_temperature->set(static_cast<int>(temperature * 100) / 100.0);

   // Generate a random variation on humidity (-1.0% to 1.0%)
   offset = static_cast<int>(shared::tools::CRandom::generate(-10, 10)) / 10.0;
   const auto humidity = m_humidity->get() + offset;

   //we keep 2 decimals
   m_humidity->set(static_cast<int>(humidity * 100) / 100.0);

   // Generate a random increment (0 to 15)
   m_incrementalCounter->set(shared::tools::CRandom::generate(0, 15));
}

void CFakeNoHistorySensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   // If you need to historize several data, use the vector form of historizeData for performance
   api->historizeData(m_deviceName, m_historizers);
}

const std::string& CFakeNoHistorySensor::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeNoHistorySensor::getModel()
{
   static const std::string model("Fake no history data");
   return model;
}


const std::string& CFakeNoHistorySensor::getType()
{
   static const std::string type("fakeNoHistorySensorType");
   return type;
}
