#include "stdafx.h"
#include "Profile_A5_10_1A.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_1A::CProfile_A5_10_1A(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_voltage(boost::make_shared<yApi::historization::CVoltage>("Supply voltage")),
     m_setPoint(boost::make_shared<yApi::historization::CTemperature>("SetPoint")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_fan(boost::make_shared<specificHistorizers::CFan6Speeds>("Fan")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_voltage, m_setPoint, m_temperature, m_fan, m_occupancy})
{
}

const std::string& CProfile_A5_10_1A::profile() const
{
   static const std::string Profile("A5-10-1A");
   return Profile;
}

const std::string& CProfile_A5_10_1A::title() const
{
   static const std::string Title(
      R"(Room Operating Panel - Supply voltage monitor, Temperature Set Point, Temperature Sensor, Fan Speed and Occupancy Control)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_1A::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_1A::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_1A::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto rawVoltage = static_cast<double>(bitset_extract(data, 0, 8));
   if (rawVoltage <= 250)
   {
      m_voltage->set(rawVoltage * 5.0 / 250.0);
      historizers.emplace_back(m_voltage);
   }

   m_setPoint->set(static_cast<double>(250 - bitset_extract(data, 8, 8)) * 40.0 / 250.0);
   historizers.emplace_back(m_setPoint);

   m_temperature->set(static_cast<double>(250 - bitset_extract(data, 16, 8)) * 40.0 / 250.0);
   historizers.emplace_back(m_temperature);

   switch (bitset_extract(data, 25, 3))
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

   if (bitset_extract(data, 30, 1))
   {
      // Occupancy sensor available
      m_occupancy->set(bitset_extract(data, 31, 1) ? true : false);
      historizers.emplace_back(m_occupancy);
   }

   return historizers;
}

void CProfile_A5_10_1A::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_1A::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Device supports no configuration
}
