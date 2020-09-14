#include "stdafx.h"
#include "Profile_A5_10_0B.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_0B::CProfile_A5_10_0B(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_contact(boost::make_shared<yApi::historization::CSwitch>("Contact")),
     m_historizers({m_temperature, m_contact})
{
}

const std::string& CProfile_A5_10_0B::profile() const
{
   static const std::string Profile("A5-10-0B");
   return Profile;
}

const std::string& CProfile_A5_10_0B::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature Sensor and Single Input Contact");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_0B::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_0B::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_0B::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_temperature->set(static_cast<double>(255 - bitset_extract(data, 16, 8)) * 40.0 / 255.0);

   m_contact->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_0B::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_0B::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
