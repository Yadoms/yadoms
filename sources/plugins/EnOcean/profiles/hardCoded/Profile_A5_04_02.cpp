#include "stdafx.h"
#include "Profile_A5_04_02.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_04_02::CProfile_A5_04_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_historizers({ m_humidity, m_temperature})
{
}

const std::string& CProfile_A5_04_02::profile() const
{
   static const std::string Profile("A5-04-02");
   return Profile;
}

const std::string& CProfile_A5_04_02::title() const
{
   static const std::string Title("Temperature (-20 to 60°C) and humidity sensor (0 to 100%)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_04_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_04_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_04_02::states(
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

   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8) * 100.0 / 250.0));
   historizers.push_back(m_humidity);

   if (bitset_extract(data, 30, 1))
   {
      // Temperature sensor available
      m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 80.0 / 250.0 - 20.0);
      historizers.push_back(m_temperature);
   }

   return historizers;
}

void CProfile_A5_04_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_04_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
