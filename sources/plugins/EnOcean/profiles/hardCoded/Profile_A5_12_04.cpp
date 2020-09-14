#include "stdafx.h"
#include "Profile_A5_12_04.h"
#include "Profile_A5_12_Common.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>

CProfile_A5_12_04::CProfile_A5_12_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_weight(boost::make_shared<yApi::historization::CWeight>("Weight")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery level")),
     m_historizers({m_weight, m_temperature, m_battery})
{
}

const std::string& CProfile_A5_12_04::profile() const
{
   static const std::string profile("A5-12-04");
   return profile;
}

const std::string& CProfile_A5_12_04::title() const
{
   static const std::string title("Automated Meter Reading - Temperature and Load Sensor");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_04::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_12_04::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_04::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   const int weightValue = bitset_extract(data, 0, 14);
   m_weight->set(static_cast<double>(weightValue) / 1000.0);

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

void CProfile_A5_12_04::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_12_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
