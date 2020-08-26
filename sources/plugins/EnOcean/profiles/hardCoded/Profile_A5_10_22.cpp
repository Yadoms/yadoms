#include "stdafx.h"
#include "Profile_A5_10_22.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_22::CProfile_A5_10_22(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_setPoint(boost::make_shared<specificHistorizers::CSetPoint>("SetPoint")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity")),
     m_fan(boost::make_shared<specificHistorizers::CFan4Speeds>("Fan")),
     m_historizers({m_setPoint, m_temperature, m_humidity, m_fan})
{
}

const std::string& CProfile_A5_10_22::profile() const
{
   static const std::string Profile("A5-10-22");
   return Profile;
}

const std::string& CProfile_A5_10_22::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature, Setpoint, Humidity and Fan Speed");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_22::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_22::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_22::states(
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

   switch (bitset_extract(status, 24, 3))
   {
   case 0:
      m_fan->set(specificHistorizers::EFan4Speeds::kAuto);
      historizers.push_back(m_fan);
      break;
   case 1:
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed0);
      historizers.push_back(m_fan);
      break;
   case 2:
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed1);
      historizers.push_back(m_fan);
      break;
   case 3:
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed2);
      historizers.push_back(m_fan);
      break;
   case 4:
      m_fan->set(specificHistorizers::EFan4Speeds::kSpeed3);
      historizers.push_back(m_fan);
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Fan speed = " << bitset_extract(status, 24, 3);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return historizers;
}

void CProfile_A5_10_22::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_22::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
