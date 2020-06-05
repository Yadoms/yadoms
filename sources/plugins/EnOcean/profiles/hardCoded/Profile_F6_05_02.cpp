#include "stdafx.h"
#include "Profile_F6_05_02.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_05_02::CProfile_F6_05_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_smoke(boost::make_shared<yApi::historization::CSwitch>("Smoke detected", yApi::EKeywordAccessMode::kGet)),
     m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("Battery", yApi::EKeywordAccessMode::kGet)),
     m_historizers({ m_smoke, m_batteryLevel})
{
}

const std::string& CProfile_F6_05_02::profile() const
{
   static const std::string Profile("F6-05-02");
   return Profile;
}

const std::string& CProfile_F6_05_02::title() const
{
   static const std::string Title("Smoke Detector");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_05_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_05_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_05_02::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   switch (bitset_to_byte(data))
   {
   case 0x00:
      m_smoke->set(false);
      m_batteryLevel->set(100);
      break;
   case 0x10:
      m_smoke->set(true);
      m_batteryLevel->set(100);
      break;
   case 0x30:
      m_batteryLevel->set(0);
      break;
   default:
      YADOMS_LOG(error) << "Profile F6_05_02 : receive unsupported value " << bitset_to_byte(data);
      break;
   }

   return m_historizers;
}

void CProfile_F6_05_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_05_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
