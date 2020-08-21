#include "stdafx.h"
#include "Profile_A5_10_0D.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_0D::CProfile_A5_10_0D(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_dayNight(boost::make_shared<yApi::historization::CSwitch>("DayNight")),
     m_historizers({m_temperature, m_dayNight})
{
}

const std::string& CProfile_A5_10_0D::profile() const
{
   static const std::string Profile("A5-10-0D");
   return Profile;
}

const std::string& CProfile_A5_10_0D::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature Sensor and Day/Night Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_0D::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_0D::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_0D::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_temperature->set(static_cast<double>(255 - bitset_extract(data, 16, 8)) * 40.0 / 255.0);

   m_dayNight->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_0D::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_0D::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
