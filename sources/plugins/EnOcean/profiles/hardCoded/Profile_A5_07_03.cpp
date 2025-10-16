#include "stdafx.h"
#include "Profile_A5_07_03.h"
#include "Profile_A5_12_Common.h"
#include "../bitsetHelpers.hpp"


CProfile_A5_07_03::CProfile_A5_07_03(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_supplyVoltage(boost::make_shared<yApi::historization::CVoltage>("Supply voltage")),
     m_illumination(boost::make_shared<yApi::historization::CIllumination>("Illumination")),
     m_pir(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_supplyVoltage, m_illumination, m_pir})
{
}

const std::string& CProfile_A5_07_03::profile() const
{
   static const std::string Profile("A5-07-03");
   return Profile;
}

const std::string& CProfile_A5_07_03::title() const
{
   static const std::string Title(R"(Occupancy Sensor - Occupancy with Supply voltage monitor and 10-bit illumination measurement)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_07_03::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_07_03::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_07_03::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   m_supplyVoltage->set(static_cast<double>(CProfile_A5_12_Common::clamp(bitset_extract(data, 0, 8),
                                                                         static_cast<unsigned>(0),
                                                                         static_cast<unsigned>(250))) * 5.0 / 250.0);

   m_illumination->set(CProfile_A5_12_Common::clamp(bitset_extract(data, 8, 10),
                                                    static_cast<unsigned>(0),
                                                    static_cast<unsigned>(1000)));

   m_pir->set(bitset_extract(data, 24, 1));

   return m_historizers;
}

void CProfile_A5_07_03::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_07_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Device supports no configuration
}
