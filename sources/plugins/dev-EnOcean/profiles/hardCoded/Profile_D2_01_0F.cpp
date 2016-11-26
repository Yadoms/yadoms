#include "stdafx.h"
#include "Profile_D2_01_0F.h"

CProfile_D2_01_0F::CProfile_D2_01_0F(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_historizers({})
{
}

CProfile_D2_01_0F::~CProfile_D2_01_0F()
{
}

const std::string& CProfile_D2_01_0F::profile() const
{
   static const std::string profile("D2-01-0F");
   return profile;
}

const std::string& CProfile_D2_01_0F::title() const
{
   static const std::string title("Slot-in module with 1 channel");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0F::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0F::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status) const
{
   //TODO
   return m_historizers;
}

void CProfile_D2_01_0F::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_01_0F::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   //TODO
}
