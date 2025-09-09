#include "stdafx.h"
#include "Profile_D2_04_06.h"

#include "profiles/eep.h"


CProfile_D2_04_06::CProfile_D2_04_06(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_common(2000.0),
     m_historizers({
        m_common.m_co2, m_common.m_day
     })
{
}

const std::string& CProfile_D2_04_06::profile() const
{
   static const std::string Profile("D2-04-06");
   return Profile;
}

const std::string& CProfile_D2_04_06::title() const
{
   static const std::string Title(R"(CO2 and Day/Night)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_06::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_04_06::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_06::states(
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

void CProfile_D2_04_06::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_04_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
