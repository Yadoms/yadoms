#include "stdafx.h"
#include "Profile_A5_09_04.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_09_04::CProfile_A5_09_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
     m_concentrationPpm(boost::make_shared<specificHistorizers::CConcentrationPpm>("Concentration(ppm)")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_historizers({ m_humidity, m_concentrationPpm, m_temperature})
{
}

const std::string& CProfile_A5_09_04::profile() const
{
   static const std::string Profile("A5-09-04");
   return Profile;
}

const std::string& CProfile_A5_09_04::title() const
{
   static const std::string Title("Gas sensor - CO2 sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_04::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_04::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_04::states(
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

   if (bitset_extract(data, 29, 1))
   {
      // Humidity sensor available
      m_humidity->set(static_cast<double>(bitset_extract(data, 0, 8)) * 100.0 / 200.0);
      historizers.push_back(m_humidity);
   }

   m_concentrationPpm->set(static_cast<double>(bitset_extract(data, 8, 8) * 2550.0 / 255.0));
   historizers.push_back(m_concentrationPpm);

   if (bitset_extract(data, 30, 1))
   {
      // Temperature sensor available
      m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 51.0 / 255.0);
      historizers.push_back(m_temperature);
   }

   return historizers;
}

void CProfile_A5_09_04::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
