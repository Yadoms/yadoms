#include "stdafx.h"
#include "Profile_F6_04_02.h"
#include "../bitsetHelpers.hpp"

CProfile_F6_04_02::CProfile_F6_04_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_cardInserted(boost::make_shared<yApi::historization::CSwitch>("Card", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_cardInserted})
{
}

const std::string& CProfile_F6_04_02::profile() const
{
   static const std::string Profile("F6-04-02");
   return Profile;
}

const std::string& CProfile_F6_04_02::title() const
{
   static const std::string Title(R"(Position switch - Key Card Activated Switch ERP2)");
   return Title;
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
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   m_cardInserted->set(bitset_extract(data, 5, 1) ? true : false);
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
