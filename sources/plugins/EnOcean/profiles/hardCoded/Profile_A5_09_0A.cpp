#include "stdafx.h"
#include "Profile_A5_09_0A.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_09_0A::CProfile_A5_09_0A(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_supplyVoltage(boost::make_shared<yApi::historization::CVoltage>("supplyVoltage")),
     m_concentrationPpm(boost::make_shared<specificHistorizers::CConcentrationPpm>("Concentration(ppm)")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature")),
     m_historizers({ m_supplyVoltage, m_concentrationPpm, m_temperature})
{
}

const std::string& CProfile_A5_09_0A::profile() const
{
   static const std::string Profile("A5-09-0A");
   return Profile;
}

const std::string& CProfile_A5_09_0A::title() const
{
   static const std::string Title(R"(Gas sensor - Hydrogen gas sensor)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_0A::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_0A::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_0A::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   if (bitset_extract(data, 31, 1))
   {
      // Supply voltage sensor available
      m_supplyVoltage->set(static_cast<double>(bitset_extract(data, 24, 4)) * 3.0 / 15.0 + 2.0);
      historizers.emplace_back(m_supplyVoltage);
   }

   m_concentrationPpm->set(bitset_extract(data, 0, 16));
   historizers.emplace_back(m_concentrationPpm);

   if (bitset_extract(data, 30, 1))
   {
      // Temperature sensor available
      m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 80.0 / 255.0 - 20.0);
      historizers.emplace_back(m_temperature);
   }

   return historizers;
}

void CProfile_A5_09_0A::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
