#include "stdafx.h"
#include "Profile_A5_10_08.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_08::CProfile_A5_10_08(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_fan(boost::make_shared<specificHistorizers::CFan4Speeds>("Fan")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_fan, m_temperature, m_occupancy})
{
}

const std::string& CProfile_A5_10_08::profile() const
{
   static const std::string Profile("A5-10-08");
   return Profile;
}

const std::string& CProfile_A5_10_08::title() const
{
   static const std::string Title(R"(Room Operating Panel - Temperature Sensor, Fan Speed and Occupancy Control)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_08::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_08::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_08::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto fan = bitset_extract(status, 0, 8);
   if (fan <= 144)
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed3);
   else if (fan <= 164)
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed2);
   else if (fan <= 189)
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed1);
   else if (fan <= 209)
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed0);
   else
      m_fan->set(specificHistorizers::EFan4Speeds::kAuto);

   m_temperature->set(static_cast<double>(255 - bitset_extract(data, 16, 8)) * 40.0 / 255.0);

   m_occupancy->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_08::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
