#include "stdafx.h"
#include "Profile_A5_07_02.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include <algorithm>

CProfile_A5_07_02::CProfile_A5_07_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_supplyVoltage(boost::make_shared<yApi::historization::CVoltage>("Supply voltage")),
     m_pir(boost::make_shared<yApi::historization::CEvent>("Motion detected", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_supplyVoltage, m_pir})
{
}

const std::string& CProfile_A5_07_02::profile() const
{
   static const std::string Profile("A5-07-02");
   return Profile;
}

const std::string& CProfile_A5_07_02::title() const
{
   static const std::string Title(
      "Occupancy Sensor - Occupancy with Supply voltage monitor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_07_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_07_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_07_02::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   m_supplyVoltage->set(
      static_cast<double>(std::max(bitset_extract(data, 0, 8), static_cast<unsigned>(250))) * 5.0 / 250.0);
   historizers.push_back(m_supplyVoltage);

   if (bitset_extract(data, 28, 1))
      historizers.push_back(m_pir);

   return m_historizers;
}

void CProfile_A5_07_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_07_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
