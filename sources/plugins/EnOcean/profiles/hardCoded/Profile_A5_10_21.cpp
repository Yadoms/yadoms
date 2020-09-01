#include "stdafx.h"
#include "Profile_A5_10_21.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_21::CProfile_A5_10_21(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_setPoint(boost::make_shared<specificHistorizers::CSetPoint>("SetPoint", yApi::EKeywordAccessMode::kGet)),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity")),
     m_heatingStates(boost::make_shared<specificHistorizers::CSpecialHeatingStates>("HeatingState")),
     m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("Battery")),
     m_userInteraction(boost::make_shared<yApi::historization::CSwitch>("UserInteraction", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_setPoint, m_temperature, m_humidity, m_heatingStates, m_batteryLevel, m_userInteraction})
{
}

const std::string& CProfile_A5_10_21::profile() const
{
   static const std::string Profile("A5-10-21");
   return Profile;
}

const std::string& CProfile_A5_10_21::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature, Humidity and Set Point with Special Heating States");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_21::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_21::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_21::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   m_setPoint->set(bitset_extract(status, 0, 8));
   historizers.push_back(m_setPoint);

   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8)) * 100.0 / 250.0);
   historizers.push_back(m_humidity);

   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 40.0 / 250.0);
   historizers.push_back(m_temperature);

   switch (bitset_extract(status, 25, 2))
   {
   case 0: m_heatingStates->set(specificHistorizers::ESpecialHeatingStates::kDefinedBySetPoint);
      historizers.push_back(m_heatingStates);
      break;
   case 1: m_heatingStates->set(specificHistorizers::ESpecialHeatingStates::kFrostProtection);
      historizers.push_back(m_heatingStates);
      break;
   case 2: m_heatingStates->set(specificHistorizers::ESpecialHeatingStates::kAutomaticControl);
      historizers.push_back(m_heatingStates);
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Special heating state = " << bitset_extract(status, 25, 2);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_batteryLevel->set(bitset_extract(status, 27, 1) ? 0 : 100);
   historizers.push_back(m_batteryLevel);

   m_userInteraction->set(bitset_extract(status, 31, 1) ? true : false);
   historizers.push_back(m_userInteraction);

   return historizers;
}

void CProfile_A5_10_21::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_21::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
