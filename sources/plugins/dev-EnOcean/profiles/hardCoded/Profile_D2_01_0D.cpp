#include "stdafx.h"
#include "Profile_D2_01_0D.h"

CProfile_D2_01_0D::CProfile_D2_01_0D(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_historizers({})
{
}

CProfile_D2_01_0D::~CProfile_D2_01_0D()
{
}

const std::string& CProfile_D2_01_0D::profile() const
{
   static const std::string profile("D2-01-0D");
   return profile;
}

const std::string& CProfile_D2_01_0D::title() const
{
   static const std::string title("Micro smart plug with 1 channel");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0D::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0D::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status) const
{
   //TODO
   return m_historizers;
}

void CProfile_D2_01_0D::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_01_0D::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   //TODO
}
