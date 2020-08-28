#include "stdafx.h"
#include "Profile_A5_30_04.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_30_04::CProfile_A5_30_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_di0(boost::make_shared<yApi::historization::CSwitch>("Digital input 0")),
     m_di1(boost::make_shared<yApi::historization::CSwitch>("Digital input 1")),
     m_di2(boost::make_shared<yApi::historization::CSwitch>("Digital input 2")),
     m_analog(boost::make_shared<specificHistorizers::CAnalog>("Analog input", 0, 255, yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_di0, m_di1, m_di2, m_analog})
{
}

const std::string& CProfile_A5_30_04::profile() const
{
   static const std::string Profile("A5-30-04");
   return Profile;
}

const std::string& CProfile_A5_30_04::title() const
{
   static const std::string Title(
      "Digital Input - 3 Digital Inputs, 1 Digital Input 8 Bits");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_30_04::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_30_04::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_30_04::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_di0->set(bitset_extract(data, 31, 1)?true:false);
   m_di1->set(bitset_extract(data, 30, 1) ? true : false);
   m_di2->set(bitset_extract(data, 29, 1) ? true : false);
   m_analog->set(bitset_extract(data, 16, 8));

   return m_historizers;
}

void CProfile_A5_30_04::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_30_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
