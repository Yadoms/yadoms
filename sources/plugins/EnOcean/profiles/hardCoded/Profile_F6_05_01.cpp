#include "stdafx.h"
#include "Profile_F6_05_01.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_05_01::CProfile_F6_05_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_waterLeakage(boost::make_shared<yApi::historization::CEvent>("Water leakage", yApi::EKeywordAccessMode::kGet)),
     m_historizers({ m_waterLeakage })
{
}

const std::string& CProfile_F6_05_01::profile() const
{
   static const std::string Profile("F6-05-01");
   return Profile;
}

const std::string& CProfile_F6_05_01::title() const
{
   static const std::string Title("Liquid Leakage Sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_05_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_05_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_05_01::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   switch (bitset_to_byte(data))
   {
   case 0x11:
      return m_historizers;
   default:
      YADOMS_LOG(error) << "Profile F6_05_01 : receive unsupported value " << bitset_to_byte(data);
      break;
   }

   return m_historizers;
}

void CProfile_F6_05_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_05_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
