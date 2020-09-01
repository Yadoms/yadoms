#include "stdafx.h"
#include "Profile_A5_30_01.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_30_01::CProfile_A5_30_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_contact(boost::make_shared<yApi::historization::CSwitch>("Contact open", yApi::EKeywordAccessMode::kGet)),
   m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery")),
     m_historizers({ m_contact, m_battery })
{
}

const std::string& CProfile_A5_30_01::profile() const
{
   static const std::string Profile("A5-30-01");
   return Profile;
}

const std::string& CProfile_A5_30_01::title() const
{
   static const std::string Title(
      "Digital Input - Single Input Contact, Battery Monitor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_30_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_30_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_30_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_contact->set(bitset_extract(data, 16, 8) >= 196);
   m_battery->set(bitset_extract(data, 8, 8) >= 121 ? 100 : 0);

   return m_historizers;
}

void CProfile_A5_30_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_30_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
