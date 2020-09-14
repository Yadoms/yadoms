#include "stdafx.h"
#include "Profile_A5_09_07.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_09_07::CProfile_A5_09_07(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_concentration10um(boost::make_shared<specificHistorizers::CConcentrationGramPerMeterCube>("lessThan10µM")),
     m_concentration2_5um(boost::make_shared<specificHistorizers::CConcentrationGramPerMeterCube>("lessThan2.5µM")),
     m_concentration1um(boost::make_shared<specificHistorizers::CConcentrationGramPerMeterCube>("lessThan1µM")),
     m_historizers({m_concentration10um, m_concentration2_5um, m_concentration1um})
{
}

const std::string& CProfile_A5_09_07::profile() const
{
   static const std::string Profile("A5-09-07");
   return Profile;
}

const std::string& CProfile_A5_09_07::title() const
{
   static const std::string Title("Gas sensor - Particles");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_07::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_07::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_07::states(
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
      // PM10 active
      m_concentration10um->set(static_cast<double>(bitset_extract(data, 0, 9)) / 1000000.0);
      historizers.push_back(m_concentration10um);
   }

   if (bitset_extract(data, 30, 1))
   {
      // PM2.5 active
      m_concentration10um->set(static_cast<double>(bitset_extract(data, 9, 9)) / 1000000.0);
      historizers.push_back(m_concentration10um);
   }

   if (bitset_extract(data, 31, 1))
   {
      // PM1 active
      m_concentration10um->set(static_cast<double>(bitset_extract(data, 18, 9)) / 1000000.0);
      historizers.push_back(m_concentration10um);
   }

   return historizers;
}

void CProfile_A5_09_07::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
