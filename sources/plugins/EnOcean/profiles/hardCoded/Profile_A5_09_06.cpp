#include "stdafx.h"
#include "Profile_A5_09_06.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>


CProfile_A5_09_06::CProfile_A5_09_06(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_volumicRadioActivity(boost::make_shared<specificHistorizers::CVolumicRadioActivity>("volumicRadioActivity")),
     m_historizers({ m_volumicRadioActivity })
{
}

const std::string& CProfile_A5_09_06::profile() const
{
   static const std::string Profile("A5-09-06");
   return Profile;
}

const std::string& CProfile_A5_09_06::title() const
{
   static const std::string Title("Gas sensor - Radon");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_06::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_06::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_06::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   m_volumicRadioActivity->set(static_cast<double>(bitset_extract(data, 0, 10)));

   return m_historizers;
}

void CProfile_A5_09_06::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
