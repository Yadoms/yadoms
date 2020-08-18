#include "stdafx.h"
#include "Profile_A5_09_0C.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>

enum E_A5_09_0C_ScaleMultiplier
{
   kA5_09_0C_Multiplier_0_01 = 0,
   kA5_09_0C_Multiplier_0_1 = 1,
   kA5_09_0C_Multiplier_1 = 2,
   kA5_09_0C_Multiplier_10 = 3,
};

enum E_A5_09_0C_Unit
{
   kA5_09_0C_Ppb = 0,
   kA5_09_0C_ugm3 = 1
};

CProfile_A5_09_0C::CProfile_A5_09_0C(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_volatileOrganicCompound(boost::make_shared<specificHistorizers::CVolatileOrganicCompound>("Compound")),
   m_concentrationPpm(boost::make_shared<specificHistorizers::CConcentrationPpm>("Concentration(ppm)")),
   m_concentrationGM3(boost::make_shared<specificHistorizers::CConcentrationGramPerMeterCube>("Concentration(g/m3)")),
   m_historizers({ m_volatileOrganicCompound, m_concentrationPpm, m_concentrationGM3 })
{
}

const std::string& CProfile_A5_09_0C::profile() const
{
   static const std::string Profile("A5-09-0C");
   return Profile;
}

const std::string& CProfile_A5_09_0C::title() const
{
   static const std::string Title("Gas sensor - VOC sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_0C::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_0C::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_0C::states(unsigned char rorg,
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
   const auto multiplier = static_cast<E_A5_09_0C_ScaleMultiplier> (bitset_extract(data, 30, 2));
   const auto unit = static_cast<E_A5_09_0C_Unit> (bitset_extract(data, 28, 1));

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   switch (multiplier)
   {
   case kA5_09_0C_Multiplier_0_01:
      concentration *= 0.01;
      break;
   case kA5_09_0C_Multiplier_0_1:
      concentration *= 0.1;
      break;
   case kA5_09_0C_Multiplier_1:
      concentration *= 1.0;
      break;
   case kA5_09_0C_Multiplier_10:
      concentration *= 10.0;
      break;
   }

   switch(unit)
   {
   case kA5_09_0C_Ppb:
      m_concentrationPpm->set(concentration / 1000.0);
      historizers.push_back(m_concentrationPpm);
      break;
   case kA5_09_0C_ugm3:
      m_concentrationGM3->set(concentration / 1000000.0);
      historizers.push_back(m_concentrationGM3);
      break;
   }

   return historizers;
}

void CProfile_A5_09_0C::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_0C::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
