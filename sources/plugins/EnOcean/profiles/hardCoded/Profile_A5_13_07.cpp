#include "stdafx.h"
#include "Profile_A5_13_07.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_07::CProfile_A5_13_07(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_direction(boost::make_shared<yApi::historization::CDirection>("Direction")),
     m_averageSpeed(boost::make_shared<yApi::historization::CSpeed>("Average speed", yApi::EKeywordAccessMode::kGet)),
     m_maximumSpeed(boost::make_shared<yApi::historization::CSpeed>("Maximum speed", yApi::EKeywordAccessMode::kGet)),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery")),
     m_historizers({m_direction, m_averageSpeed, m_maximumSpeed, m_battery})
{
}

const std::string& CProfile_A5_13_07::profile() const
{
   static const std::string Profile("A5-13-07");
   return Profile;
}

const std::string& CProfile_A5_13_07::title() const
{
   static const std::string Title("Environmental Applications - Wind Sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_07::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_07::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_07::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   switch (bitset_extract(data, 4, 4))
   {
   case 0: //NNE
      m_direction->set(22);
      break;
   case 1: //NE
      m_direction->set(45);
      break;
   case 2: //ENE
      m_direction->set(67);
      break;
   case 3: //E
      m_direction->set(90);
      break;
   case 4: //ESE
      m_direction->set(112);
      break;
   case 5: //SE
      m_direction->set(135);
      break;
   case 6: //SSE
      m_direction->set(157);
      break;
   case 7: //S
      m_direction->set(180);
      break;
   case 8: //SSW
      m_direction->set(202);
      break;
   case 9: //SW
      m_direction->set(225);
      break;
   case 10: //WSW
      m_direction->set(247);
      break;
   case 11: //W
      m_direction->set(270);
      break;
   case 12: //WNW
      m_direction->set(292);
      break;
   case 13: //NW
      m_direction->set(315);
      break;
   case 14: //NNW
      m_direction->set(337);
      break;
   case 15: //N
      m_direction->set(0);
      break;
   default:
      break;
   }

   m_averageSpeed->set((static_cast<double>(bitset_extract(data, 8, 8)) * 198.9 / 255.0 + 1.0) / 2.237);
   m_maximumSpeed->set((static_cast<double>(bitset_extract(data, 16, 8)) * 198.9 / 255.0 + 1.0) / 2.237);

   m_battery->set(bitset_extract(data, 31, 1) ? 0 : 100);

   return m_historizers;
}

void CProfile_A5_13_07::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}

