#include "stdafx.h"
#include "Profile_F6_04_01.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_04_01::CProfile_F6_04_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_cardInserted(boost::make_shared<yApi::historization::CSwitch>("Card", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_cardInserted})
{
}

const std::string& CProfile_F6_04_01::profile() const
{
   static const std::string Profile("F6-04-01");
   return Profile;
}

const std::string& CProfile_F6_04_01::title() const
{
   static const std::string Title(R"(Position switch - Key Card Activated Switch)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_04_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_04_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_04_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   const auto T21 = bitset_extract(status, 2, 1) ? true : false;
   const auto NU = bitset_extract(status, 3, 1) ? true : false;
   const auto keyCardInserted = bitset_extract(data, 0, 8) == 0x70;

   if (T21 && NU && keyCardInserted)
      m_cardInserted->set(true);
   else if (T21 && !NU && !keyCardInserted)
      m_cardInserted->set(false);
   else
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : T21=" << (T21 ? "1" : "0") <<
         ", NU=" << (NU ? "1" : "0") <<
         ", keyCardAction=" << keyCardInserted;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return m_historizers;
}

void CProfile_F6_04_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_04_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Device supports no configuration
}
