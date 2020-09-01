#include "stdafx.h"
#include "Profile_A5_10_03.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_03::CProfile_A5_10_03(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_setPoint(boost::make_shared<specificHistorizers::CSetPoint>("SetPoint", yApi::EKeywordAccessMode::kGet)),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_historizers({ m_setPoint, m_temperature})
{
}

const std::string& CProfile_A5_10_03::profile() const
{
   static const std::string Profile("A5-10-03");
   return Profile;
}

const std::string& CProfile_A5_10_03::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature Sensor, Set Point Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_03::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_03::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_03::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_setPoint->set(bitset_extract(status, 8, 8));

   m_temperature->set(static_cast<double>(255 - bitset_extract(data, 16, 8)) * 40.0 / 255.0);

   return m_historizers;
}

void CProfile_A5_10_03::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
