#include "stdafx.h"
#include "Profile_A5_13_08.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_08::CProfile_A5_13_08(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_rain(boost::make_shared<yApi::historization::CRain>("Rain")),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery")),
     m_historizers({m_rain, m_battery})
{
}

const std::string& CProfile_A5_13_08::profile() const
{
   static const std::string Profile("A5-13-08");
   return Profile;
}

const std::string& CProfile_A5_13_08::title() const
{
   static const std::string Title("Environmental Applications - Rain Sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_08::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_08::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_08::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto rainFallAdjustSign = bitset_extract(data, 1, 1) ? 1.0 : -1.0;
   const auto rainFallAdjust = static_cast<double>(bitset_extract(data, 2, 6));
   const auto rainFallCount = static_cast<double>(bitset_extract(data, 8, 16));

   m_rain->set(rainFallCount * 0.6875 * (1.0 + rainFallAdjustSign * rainFallAdjust));

   m_battery->set(bitset_extract(data, 31, 1) ? 0 : 100);

   return m_historizers;
}

void CProfile_A5_13_08::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
