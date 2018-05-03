#include "stdafx.h"
#include "Profile_A5_12_05.h"
#include "Profile_A5_12_Common.h"
#include "profiles/eep.h"
#include "profiles/bitsetHelpers.hpp"

CProfile_A5_12_05::CProfile_A5_12_05(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery level")),
     m_historizers({m_temperature, m_battery})
{
   for (auto indexPossessed = 0; indexPossessed < NB_POSSESSED; ++indexPossessed)
   {
      const auto strPossessed = std::to_string(indexPossessed);
      m_possessed[indexPossessed] = boost::make_shared<yApi::historization::CSwitch>("Possessed (" + strPossessed + ")");
      m_historizers.push_back(m_possessed[indexPossessed]);
   }
}

CProfile_A5_12_05::~CProfile_A5_12_05()
{
}

const std::string& CProfile_A5_12_05::profile() const
{
   static const std::string profile("A5-12-05");
   return profile;
}

const std::string& CProfile_A5_12_05::title() const
{
   static const std::string title("Automated Meter Reading - Temperature and Container Sensor");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_05::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_12_05::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_05::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   for (auto indexPossessed = 0; indexPossessed < NB_POSSESSED; ++indexPossessed)
   {
      m_possessed[indexPossessed]->set(bitset_extract(data, indexPossessed, 1) ? true : false);
   }

   const int temperatureValue = bitset_extract(data, 16, 8);
   m_temperature->set(((40.0 - -40.0) / 255.0) * temperatureValue - 40.0);

   const int batteryLevelValue = bitset_extract(data, 30, 2);
   switch (batteryLevelValue)
   {
   case 0:
      m_battery->set(100);
      break;
   case 1:
      m_battery->set(75);
      break;
   case 2:
      m_battery->set(50);
      break;
   default:
      m_battery->set(25);
      break;
   }

   return m_historizers;
}

void CProfile_A5_12_05::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_12_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
