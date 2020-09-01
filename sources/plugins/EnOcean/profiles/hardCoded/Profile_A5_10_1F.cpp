#include "stdafx.h"
#include "Profile_A5_10_1F.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_10_1F::CProfile_A5_10_1F(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_fan(boost::make_shared<specificHistorizers::CFan4Speeds>("Fan")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_setPoint(boost::make_shared<yApi::historization::CTemperature>("SetPoint")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({ m_fan, m_temperature, m_setPoint, m_occupancy})
{
}

const std::string& CProfile_A5_10_1F::profile() const
{
   static const std::string Profile("A5-10-1F");
   return Profile;
}

const std::string& CProfile_A5_10_1F::title() const
{
   static const std::string Title(
      "Room Operating Panel - Temperature Sensor, Set Point, Fan Speed, Occupancy and Unoccupancy Control");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_1F::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_10_1F::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_10_1F::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   if (bitset_extract(status, 27, 1))
   {
      const auto fan = bitset_extract(status, 0, 8);
      if (fan <= 144)
         m_fan->set(specificHistorizers::EFan4Speeds::kSpeed3);
      else if (fan <= 164)
         m_fan->set(specificHistorizers::EFan4Speeds::kSpeed2);
      else if (fan <= 189)
         m_fan->set(specificHistorizers::EFan4Speeds::kSpeed1);
      else if (fan <= 209)
         m_fan->set(specificHistorizers::EFan4Speeds::kSpeed0);
      else
         m_fan->set(specificHistorizers::EFan4Speeds::kAuto);
      historizers.push_back(m_fan);
   }

   if (bitset_extract(status, 27, 1))
   {
      m_temperature->set(static_cast<double>(250 - bitset_extract(data, 16, 8)) * 40.0 / 250.0);
      historizers.push_back(m_temperature);
   }

   if (bitset_extract(status, 26, 1))
   {
      m_setPoint->set(bitset_extract(status, 8, 8));
      historizers.push_back(m_setPoint);
   }

   if (bitset_extract(status, 31, 1))
   {
      m_occupancy->set(true);
      historizers.push_back(m_occupancy);
   }
   else if (bitset_extract(status, 30, 1))
   {
      m_occupancy->set(false);
      historizers.push_back(m_occupancy);
   }

   return historizers;
}

void CProfile_A5_10_1F::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_10_1F::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
