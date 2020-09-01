#include "stdafx.h"
#include "Profile_A5_10_1E.h"
#include "../bitsetHelpers.hpp"
#include <algorithm>

CProfile_A5_10_1E::CProfile_A5_10_1E(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_voltage(boost::make_shared<yApi::historization::CVoltage>("Supply voltage")),
     m_illumination(boost::make_shared<yApi::historization::CIllumination>("Illumination")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_fan(boost::make_shared<specificHistorizers::CFan6Speeds>("Fan")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_voltage, m_illumination, m_temperature, m_fan, m_occupancy})
{
}

const std::string& CProfile_A5_10_1E::profile() const
{
   static const std::string Profile("A5-10-1E");
   return Profile;
}

const std::string& CProfile_A5_10_1E::title() const
{
   static const std::string Title(
      "Room Operating Panel - Supply Voltage Monitor, Illumination, Temperature Sensor, Fan Speed and Occupancy Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_1E::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_1E::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_1E::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto rawVoltage = static_cast<double>(bitset_extract(data, 0, 8));
   if (rawVoltage <= 250)
   {
      m_voltage->set(rawVoltage * 5.0 / 250.0);
      historizers.push_back(m_voltage);
   }

   m_illumination->set(
      static_cast<double>(std::max(bitset_extract(data, 8, 8), static_cast<unsigned>(250))) * 1000.0 / 250.0);
   historizers.push_back(m_illumination);

   m_temperature->set(static_cast<double>(250 - bitset_extract(data, 16, 8)) * 40.0 / 250.0);
   historizers.push_back(m_temperature);

   switch (bitset_extract(status, 25, 3))
   {
   case 0:
      m_fan->set(specificHistorizers::EFan6Speeds::kAuto);
      break;
   case 1:
      m_fan->set(specificHistorizers::EFan6Speeds::kSpeed0);
      break;
   case 2:
      m_fan->set(specificHistorizers::EFan6Speeds::kSpeed1);
      break;
   case 3:
      m_fan->set(specificHistorizers::EFan6Speeds::kSpeed2);
      break;
   case 4:
      m_fan->set(specificHistorizers::EFan6Speeds::kSpeed3);
      break;
   case 5:
      m_fan->set(specificHistorizers::EFan6Speeds::kSpeed4);
      break;
   case 6:
      m_fan->set(specificHistorizers::EFan6Speeds::kSpeed5);
      break;
   default:
      m_fan->set(specificHistorizers::EFan6Speeds::kOff);
      break;
   }
   historizers.push_back(m_fan);

   if (bitset_extract(status, 30, 1))
   {
      // Occupancy sensor available
      m_occupancy->set(bitset_extract(status, 31, 1) ? true : false);
      historizers.push_back(m_occupancy);
   }

   return historizers;
}

void CProfile_A5_10_1E::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_1E::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
