#include "stdafx.h"
#include "Profile_F6_04_02.h"
#include "../bitsetHelpers.hpp"

CProfile_F6_04_02::CProfile_F6_04_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_cardAction(boost::make_shared<yApi::historization::CSwitch>("Button A", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_cardAction})
{
}

const std::string& CProfile_F6_04_02::profile() const
{
   static const std::string profile("F6-04-02");
   return profile;
}

const std::string& CProfile_F6_04_02::title() const
{
   static const std::string title("Position switch - Key Card Activated Switch ERP2");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_04_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_04_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_04_02::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_cardAction->set(bitset_extract(status, 5, 1) ? true : false);
   return m_historizers;
}

void CProfile_F6_04_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_04_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
