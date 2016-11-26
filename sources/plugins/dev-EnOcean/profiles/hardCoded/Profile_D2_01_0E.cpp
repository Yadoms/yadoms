#include "stdafx.h"
#include "Profile_D2_01_0E.h"

CProfile_D2_01_0E::CProfile_D2_01_0E(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_historizers({})
{
}

CProfile_D2_01_0E::~CProfile_D2_01_0E()
{
}

const std::string& CProfile_D2_01_0E::profile() const
{
   static const std::string profile("D2-01-0E");
   return profile;
}

const std::string& CProfile_D2_01_0E::title() const
{
   static const std::string title("Micro smart plug with 1 channel, and metering capabilities");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0E::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0E::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status) const
{
   //TODO
   return m_historizers;
}

void CProfile_D2_01_0E::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_01_0E::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   //TODO
}
