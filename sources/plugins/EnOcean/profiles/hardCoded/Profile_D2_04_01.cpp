#include "stdafx.h"
#include "Profile_D2_04_01.h"

#include "profiles/eep.h"


CProfile_D2_04_01::CProfile_D2_04_01(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_common(2000.0),
     m_historizers({
        m_common.m_humidity, m_common.m_co2, m_common.m_day, m_common.m_battery
     })
{
}

const std::string& CProfile_D2_04_01::profile() const
{
   static const std::string Profile("D2-04-01");
   return Profile;
}

const std::string& CProfile_D2_04_01::title() const
{
   static const std::string Title(R"(CO2, Humidity, Day/Night and Autonomy)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_04_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   m_common.states(data,
                   status);

   return m_historizers;
}

void CProfile_D2_04_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_04_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
