#include "stdafx.h"
#include "Profile_A5_10_13.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_13::CProfile_A5_10_13(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_humidity, m_temperature, m_occupancy})
{
}

const std::string& CProfile_A5_10_13::profile() const
{
   static const std::string Profile("A5-10-13");
   return Profile;
}

const std::string& CProfile_A5_10_13::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature and Humidity Sensor, Occupancy Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_13::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_13::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_13::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8)) * 100.0 / 250.0);

   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 40.0 / 250.0);

   m_occupancy->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_13::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_13::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
