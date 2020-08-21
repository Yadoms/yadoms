#include "stdafx.h"
#include "Profile_A5_10_14.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_14::CProfile_A5_10_14(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_dayNight(boost::make_shared<yApi::historization::CSwitch>("DayNight")),
     m_historizers({m_humidity, m_temperature, m_dayNight })
{
}

const std::string& CProfile_A5_10_14::profile() const
{
   static const std::string Profile("A5-10-14");
   return Profile;
}

const std::string& CProfile_A5_10_14::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature and Humidity Sensor, Day/Night Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_14::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_14::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_14::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8)) * 100.0 / 250.0);

   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 40.0 / 250.0);

   m_dayNight->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_14::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_14::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
