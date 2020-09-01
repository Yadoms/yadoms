#include "stdafx.h"
#include "Profile_A5_14_09.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_14_09::CProfile_A5_14_09(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_supplyVoltage(boost::make_shared<yApi::historization::CVoltage>("Supply voltage")),
     m_contact(boost::make_shared<yApi::historization::CSwitch>("Contact open", yApi::EKeywordAccessMode::kGet)),
     m_tilt(boost::make_shared<yApi::historization::CEvent>("Tilt", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_supplyVoltage, m_contact, m_tilt})
{
}

const std::string& CProfile_A5_14_09::profile() const
{
   static const std::string Profile("A5-04-09");
   return Profile;
}

const std::string& CProfile_A5_14_09::title() const
{
   static const std::string Title(
      "Multi-Func Sensor - Window/Door-Sensor with States Open/Closed/Tilt, Supply voltage monitor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_14_09::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_14_09::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_14_09::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   m_supplyVoltage->set(static_cast<double>(bitset_extract(data, 0, 8) * 5.0 / 250.0));
   historizers.push_back(m_supplyVoltage);

   switch (bitset_extract(data, 29, 2))
   {
   case 0:
      m_contact->set(false);
      historizers.push_back(m_contact);
      break;
   case 1:
      historizers.push_back(m_tilt);
      break;
   case 3:
      m_contact->set(true);
      historizers.push_back(m_contact);
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Contact=" << bitset_extract(data, 29, 2);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return historizers;
}

void CProfile_A5_14_09::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_14_09::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
