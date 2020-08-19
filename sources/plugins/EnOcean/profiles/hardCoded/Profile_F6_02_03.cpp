#include "stdafx.h"
#include "Profile_F6_02_03.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_02_03::CProfile_F6_02_03(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_autoMode(boost::make_shared<yApi::historization::CSwitch>("Auto mode", yApi::EKeywordAccessMode::kGet)),
     m_upDown(boost::make_shared<yApi::historization::CSwitch>("Dim to up(1)/down(0)", yApi::EKeywordAccessMode::kGet)),
     m_manualOnOff(
        boost::make_shared<yApi::historization::CEvent>("Manual state toggle", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_autoMode, m_upDown, m_manualOnOff})
{
}

CProfile_F6_02_03::~CProfile_F6_02_03()
{
}

const std::string& CProfile_F6_02_03::profile() const
{
   static const std::string profile("F6-02-03");
   return profile;
}

const std::string& CProfile_F6_02_03::title() const
{
   static const std::string title("Rocker switch, 2 rockers - Light control, application style 1");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_02_03::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_02_03::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_02_03::states(unsigned char rorg,
                                                                                                   const boost::
                                                                                                   dynamic_bitset<>&
                                                                                                   data,
                                                                                                   const boost::
                                                                                                   dynamic_bitset<>&
                                                                                                   status,
                                                                                                   const std::string&
                                                                                                   senderId,
                                                                                                   boost::shared_ptr<
                                                                                                      IMessageHandler>
                                                                                                   messageHandler) const
{
   const auto T21 = bitset_extract(status, 2, 1) ? true : false;
   const auto NU = bitset_extract(status, 3, 1) ? true : false;

   if (!T21)
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

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto action = bitset_extract(data, 0, 8);
   switch (action)
   {
   case 0x30:
      m_autoMode->set(true);
      historizers.push_back(m_autoMode);
      break;
   case 0x10:
      m_autoMode->set(false);
      historizers.push_back(m_autoMode);
      historizers.push_back(m_manualOnOff);
      break;
   case 0x70:
      m_upDown->set(true);
      historizers.push_back(m_upDown);
      break;
   case 0x50:
      m_upDown->set(false);
      historizers.push_back(m_upDown);
      break;
   default:
      YADOMS_LOG(error) << "Profile F6_02_03 : receive unsupported rocker action value " << action;
      break;
   }

   return historizers;
}

void CProfile_F6_02_03::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_02_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
