#include "stdafx.h"
#include "Profile_A5_10_0A.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_0A::CProfile_A5_10_0A(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_setPoint(boost::make_shared<specificHistorizers::CSetPoint>("SetPoint")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_contact(boost::make_shared<yApi::historization::CSwitch>("Contact")),
     m_historizers({m_setPoint, m_temperature, m_contact})
{
}

const std::string& CProfile_A5_10_0A::profile() const
{
   static const std::string Profile("A5-10-0A");
   return Profile;
}

const std::string& CProfile_A5_10_0A::title() const
{
   static const std::string Title(R"(Room Operating Panel - Temperature Sensor, Set Point Adjust and Single Input Contact)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_0A::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_0A::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_0A::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   m_setPoint->set(bitset_extract(data, 8, 8));

   m_temperature->set(static_cast<double>(255 - bitset_extract(data, 16, 8)) * 40.0 / 255.0);

   m_contact->set(bitset_extract(data, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_10_0A::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Device supports no configuration
}
