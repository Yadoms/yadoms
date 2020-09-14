#include "stdafx.h"
#include "Profile_A5_09_08.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_09_08::CProfile_A5_09_08(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_concentrationPpm(boost::make_shared<specificHistorizers::CConcentrationPpm>("Concentration(ppm)")),
     m_historizers({m_concentrationPpm})
{
}

const std::string& CProfile_A5_09_08::profile() const
{
   static const std::string Profile("A5-09-08");
   return Profile;
}

const std::string& CProfile_A5_09_08::title() const
{
   static const std::string Title("Gas sensor - Pure CO2 sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_08::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_08::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_08::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   m_concentrationPpm->set(static_cast<double>(bitset_extract(data, 16, 8) * 2000.0 / 255.0));

   return m_historizers;
}

void CProfile_A5_09_08::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
