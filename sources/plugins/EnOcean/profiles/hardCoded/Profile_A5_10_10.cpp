#include "stdafx.h"
#include "Profile_A5_10_10.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_10::CProfile_A5_10_10(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_setPoint(boost::make_shared<specificHistorizers::CSetPoint>("SetPoint", yApi::EKeywordAccessMode::kGet)),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_setPoint, m_humidity, m_temperature, m_occupancy})
{
}

const std::string& CProfile_A5_10_10::profile() const
{
   static const std::string Profile("A5-10-10");
   return Profile;
}

const std::string& CProfile_A5_10_10::title() const
{
   static const std::string Title(R"(Room Operating Panel - Temperature and Humidity Sensor, Set Point and Occupancy Control)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_10::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_10::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_10::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_setPoint->set(bitset_extract(status, 0, 8));

   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8)) * 100.0 / 250.0);

   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 40.0 / 250.0);

   m_occupancy->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_10::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
