#include "stdafx.h"
#include "Profile_A5_30_05.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_30_05::CProfile_A5_30_05(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_supplyVoltage(boost::make_shared<yApi::historization::CVoltage>("Supply voltage")),
     m_button(boost::make_shared<yApi::historization::CEvent>("Button", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_supplyVoltage, m_button})
{
}

const std::string& CProfile_A5_30_05::profile() const
{
   static const std::string Profile("A5-30-05");
   return Profile;
}

const std::string& CProfile_A5_30_05::title() const
{
   static const std::string Title(
      "Digital Input - Single Input Contact, Retransmission, Battery Monitor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_30_05::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_30_05::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_30_05::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   m_supplyVoltage->set(static_cast<double>(bitset_extract(data, 8, 8)) * 3.3 / 255.0);
   historizers.push_back(m_supplyVoltage);

   if (bitset_extract(data, 16, 1) == 0)
      historizers.push_back(m_button);

   return m_historizers;
}

void CProfile_A5_30_05::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_30_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
