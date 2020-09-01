#include "stdafx.h"
#include "Profile_A5_10_16.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_16::CProfile_A5_10_16(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_setPoint(boost::make_shared<specificHistorizers::CSetPoint>("SetPoint", yApi::EKeywordAccessMode::kGet)),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_setPoint, m_temperature, m_occupancy})
{
}

const std::string& CProfile_A5_10_16::profile() const
{
   static const std::string Profile("A5-10-16");
   return Profile;
}

const std::string& CProfile_A5_10_16::title() const
{
   static const std::string Title(
      "Room Operating Panel - 10 Bit Temperature Sensor, 6 bit Set Point Control and Occupancy Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_16::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_16::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_16::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_setPoint->set(bitset_extract(status, 8, 6));

   m_temperature->set(static_cast<double>(1023 - bitset_extract(data, 14, 10)) * 51.2 / 1023.0 - 10.0);

   m_occupancy->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_16::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_16::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
