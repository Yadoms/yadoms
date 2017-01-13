#include "stdafx.h"
#include "Profile_F6_02_02.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_02_02::CProfile_F6_02_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_buttonA(boost::make_shared<yApi::historization::CSwitch>("Button A")),
     m_buttonB(boost::make_shared<yApi::historization::CSwitch>("Button B")),
     m_buttonA2ndAction(boost::make_shared<yApi::historization::CSwitch>("Button A - 2nd action")),
     m_buttonB2ndAction(boost::make_shared<yApi::historization::CSwitch>("Button B - 2nd action")),
     m_historizers({m_buttonA , m_buttonB}),
     m_secondActionHistorizers({m_buttonA2ndAction , m_buttonB2ndAction})
{
}

CProfile_F6_02_02::~CProfile_F6_02_02()
{
}

const std::string& CProfile_F6_02_02::profile() const
{
   static const std::string profile("F6-02-02");
   return profile;
}

const std::string& CProfile_F6_02_02::title() const
{
   static const std::string title("Rocker switch, 2 rockers");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_02_02::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_02_02::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status) const
{
   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   auto T21 = bitset_extract(status, 2, 1) ? true : false;
   auto NU = bitset_extract(status, 3, 1) ? true : false;

   if (!T21)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : T21=" << (T21 ? "1" : "0") ;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
   if (!NU)
   {
      YADOMS_LOG(information) << "Message received for profile " << profile() << " with NU=0 : this message contains nothing useful, will be ignored" ;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   auto nenergyBow = bitset_extract(data, 3, 1) ? true : false;
   if (!nenergyBow)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   auto rocker1stAction = bitset_extract(data, 0, 3);
   switch (rocker1stAction)
   {
   case 0:
      m_buttonA->set(true);
      historizers.push_back(m_buttonA);
      break;
   case 1:
      m_buttonA->set(false);
      historizers.push_back(m_buttonA);
      break;
   case 2:
      m_buttonB->set(true);
      historizers.push_back(m_buttonB);
      break;
   case 3:
      m_buttonB->set(false);
      historizers.push_back(m_buttonB);
      break;
   default:
      YADOMS_LOG(error) << "Profile F6_02_02 : receive unsupported rocker first action value " << rocker1stAction ;
      break;
   }

   auto secondAction = bitset_extract(data, 7, 1) ? true : false;
   if (secondAction)
   {
      // 2nd action historizers are perhaps not declared
      m_api->declareKeywords(m_deviceId,
                             m_secondActionHistorizers);

      auto rocker2ndAction = bitset_extract(data, 4, 3);
      switch (rocker2ndAction)
      {
      case 0:
         m_buttonA2ndAction->set(true);
         historizers.push_back(m_buttonA2ndAction);
         break;
      case 1:
         m_buttonA2ndAction->set(false);
         historizers.push_back(m_buttonA2ndAction);
         break;
      case 2:
         m_buttonB2ndAction->set(true);
         historizers.push_back(m_buttonB2ndAction);
         break;
      case 3:
         m_buttonB2ndAction->set(false);
         historizers.push_back(m_buttonB2ndAction);
         break;
      default:
         YADOMS_LOG(error) << "Profile F6_02_02 : receive unsupported rocker second action value " << rocker2ndAction ;
         break;
      }
   }

   return historizers;
}

void CProfile_F6_02_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_02_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
