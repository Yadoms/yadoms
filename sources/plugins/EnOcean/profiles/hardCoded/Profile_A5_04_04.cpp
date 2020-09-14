#include "stdafx.h"
#include "Profile_A5_04_04.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_04_04::CProfile_A5_04_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_historizers({m_humidity, m_temperature})
{
}

const std::string& CProfile_A5_04_04::profile() const
{
   static const std::string Profile("A5-04-04");
   return Profile;
}

const std::string& CProfile_A5_04_04::title() const
{
   static const std::string Title("Temperature (-40 to 120°C) and humidity sensor (0 to 100%)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_04_04::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_04_04::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_04_04::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto humidity = bitset_extract(data, 0, 8);
   if (humidity < 200)
   {
      m_humidity->set(static_cast<double>(humidity) * 100.0 / 199.0);
      historizers.push_back(m_humidity);
   }

   const auto temperature = bitset_extract(data, 12, 12);
   if (temperature < 1600)
   {
      m_temperature->set(static_cast<double>(temperature) * 160.0 / 1599.0 - 40.0);
      historizers.push_back(m_temperature);
   }

   return historizers;
}

void CProfile_A5_04_04::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_04_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
