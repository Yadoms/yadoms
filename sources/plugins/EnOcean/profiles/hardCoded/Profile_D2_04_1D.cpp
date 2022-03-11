#include "stdafx.h"
#include "Profile_D2_04_1D.h"

#include "profiles/eep.h"


CProfile_D2_04_1D::CProfile_D2_04_1D(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_common(5000.0),
     m_historizers({
        m_common.m_co2, m_common.m_day
     })
{
}

const std::string& CProfile_D2_04_1D::profile() const
{
   static const std::string Profile("D2-04-1D");
   return Profile;
}

const std::string& CProfile_D2_04_1D::title() const
{
   static const std::string Title(R"(CO2 and Day/Night)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_1D::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_04_1D::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_1D::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_common.states(data,
                   status);

   return m_historizers;
}

void CProfile_D2_04_1D::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_04_1D::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
