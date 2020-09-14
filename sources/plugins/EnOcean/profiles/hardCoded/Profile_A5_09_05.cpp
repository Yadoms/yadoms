#include "stdafx.h"
#include "Profile_A5_09_05.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>

enum E_A5_09_05_ScaleMultiplier
{
   kA5_09_05_Multiplier_0_01 = 0,
   kA5_09_05_Multiplier_0_1 = 1,
   kA5_09_05_Multiplier_1 = 2,
   kA5_09_05_Multiplier_10 = 3,
};

CProfile_A5_09_05::CProfile_A5_09_05(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_volatileOrganicCompound(boost::make_shared<specificHistorizers::CVolatileOrganicCompound>("Compound")),
   m_concentration(boost::make_shared<specificHistorizers::CConcentrationPpm>("ConcentrationPpm")),
   m_historizers({ m_volatileOrganicCompound, m_concentration })
{
}

const std::string& CProfile_A5_09_05::profile() const
{
   static const std::string Profile("Gas sensor - VOC sensor");
   return Profile;
}

const std::string& CProfile_A5_09_05::title() const
{
   static const std::string Title("Gas sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_05::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_05::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_05::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   m_volatileOrganicCompound->set(specificHistorizers::EVolatileOrganicCompound(bitset_extract(data, 16, 8)));

   auto concentration = static_cast<double>(bitset_extract(data, 0, 16));
   const auto multiplier = static_cast<E_A5_09_05_ScaleMultiplier> (bitset_extract(data, 30, 2));

   switch (multiplier)
   {
   case kA5_09_05_Multiplier_0_01:
      concentration *= 0.01;
      break;
   case kA5_09_05_Multiplier_0_1:
      concentration *= 0.1;
      break;
   case kA5_09_05_Multiplier_1:
      concentration *= 1.0;
      break;
   case kA5_09_05_Multiplier_10:
      concentration *= 10.0;
      break;
   }

   m_concentration->set(concentration);

   return m_historizers;
}

void CProfile_A5_09_05::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
