#include "stdafx.h"
#include "Profile_D2_04_08.h"

#include <utility>
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_D2_04_08::CProfile_D2_04_08(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(std::move(deviceId)),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature",
                                                                         yApi::EKeywordAccessMode::kGet)),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity",
                                                                   yApi::EKeywordAccessMode::kGet)),
     m_co2(boost::make_shared<specificHistorizers::CConcentrationPpm>("Concentration(ppm)",
                                                                      yApi::EKeywordAccessMode::kGet)),
     m_day(boost::make_shared<yApi::historization::CSwitch>("Day",
                                                            yApi::EKeywordAccessMode::kGet)),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery",
                                                                      yApi::EKeywordAccessMode::kGet)),
     m_historizers({
        m_temperature, m_humidity, m_co2, m_day, m_battery
     })
{
}

const std::string& CProfile_D2_04_08::profile() const
{
   static const std::string Profile("D2-04-08");
   return Profile;
}

const std::string& CProfile_D2_04_08::title() const
{
   static const std::string Title(R"(CO2, Humidity, Temperature, Day/Night and Autonomy)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_08::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_04_08::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_04_08::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 51.0 / 255.0);

   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8)) * 100.0 / 200.0);

   m_co2->set(static_cast<double>(bitset_extract(data, 0, 8)) * 2000.0 / 255.0);

   m_day->set(bitset_extract(status, 24, 1) ? false : true);

   switch (bitset_extract(data, 25, 3))
   {
   case 0:
      m_battery->set(100);
      break;
   case 1:
      m_battery->set(87);
      break;
   case 2:
      m_battery->set(75);
      break;
   case 3:
      m_battery->set(62);
      break;
   case 4:
      m_battery->set(50);
      break;
   case 5:
      m_battery->set(37);
      break;
   case 6:
      m_battery->set(25);
      break;
   case 7:
      m_battery->set(12);
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Battery=" << bitset_extract(data, 25, 3);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return m_historizers;
}

void CProfile_D2_04_08::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_04_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
