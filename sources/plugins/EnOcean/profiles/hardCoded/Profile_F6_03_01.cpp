#include "stdafx.h"
#include "Profile_F6_03_01.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_03_01::CProfile_F6_03_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_buttonA(boost::make_shared<yApi::historization::CSwitch>("Button A", yApi::EKeywordAccessMode::kGet)),
     m_buttonB(boost::make_shared<yApi::historization::CSwitch>("Button B", yApi::EKeywordAccessMode::kGet)),
     m_buttonC(boost::make_shared<yApi::historization::CSwitch>("Button C", yApi::EKeywordAccessMode::kGet)),
     m_buttonD(boost::make_shared<yApi::historization::CSwitch>("Button D", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_buttonA, m_buttonB, m_buttonC, m_buttonD})
{
}

const std::string& CProfile_F6_03_01::profile() const
{
   static const std::string profile("F6-03-01");
   return profile;
}

const std::string& CProfile_F6_03_01::title() const
{
   static const std::string title("Rocker switch, 4 rocker - Light and Blind Control - Application style 1");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_03_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_03_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_03_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto T21 = bitset_extract(status, 2, 1) ? true : false;
   const auto NU = bitset_extract(status, 3, 1) ? true : false;

   if (T21)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : T21=" << (T21 ? "1" : "0");
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
   if (!NU)
   {
      YADOMS_LOG(information) << "Message received for profile " << profile() <<
         " with NU=0 : this message contains nothing useful, will be ignored";
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   const auto energyBow = bitset_extract(data, 3, 1) ? true : false;
   if (!energyBow)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   const auto rocker1stAction = bitset_extract(data, 0, 3);
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
   case 4:
      m_buttonC->set(false);
      historizers.push_back(m_buttonC);
      break;
   case 5:
      m_buttonC->set(false);
      historizers.push_back(m_buttonC);
      break;
   case 6:
      m_buttonD->set(false);
      historizers.push_back(m_buttonD);
      break;
   case 7:
      m_buttonD->set(false);
      historizers.push_back(m_buttonD);
      break;
   default:
      YADOMS_LOG(error) << "Profile F6_03_01 : receive unsupported rocker first action value " << rocker1stAction;
      break;
   }

   const auto secondAction = bitset_extract(data, 7, 1) ? true : false;
   if (secondAction)
   {
      const auto rocker2ndAction = bitset_extract(data, 4, 3);
      switch (rocker2ndAction)
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
      case 4:
         m_buttonC->set(true);
         historizers.push_back(m_buttonC);
         break;
      case 5:
         m_buttonC->set(false);
         historizers.push_back(m_buttonC);
         break;
      case 6:
         m_buttonD->set(true);
         historizers.push_back(m_buttonD);
         break;
      case 7:
         m_buttonD->set(false);
         historizers.push_back(m_buttonD);
         break;
      default:
         YADOMS_LOG(error) << "Profile F6_03_01 : receive unsupported rocker second action value " << rocker2ndAction;
         break;
      }
   }

   return historizers;
}

void CProfile_F6_03_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_03_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}