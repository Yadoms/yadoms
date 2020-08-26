#include "stdafx.h"
#include "Profile_A5_09_0B.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>

enum E_A5_09_0B_ScaleMultiplier
{
   kA5_09_0B_Multiplier_0_001 = 0,
   kA5_09_0B_Multiplier_0_01 = 1,
   kA5_09_0B_Multiplier_0_1 = 2,
   kA5_09_0B_Multiplier_1 = 3,
   kA5_09_0B_Multiplier_10 = 4,
   kA5_09_0B_Multiplier_100 = 5,
   kA5_09_0B_Multiplier_1000 = 6,
   kA5_09_0B_Multiplier_10000 = 7,
   kA5_09_0B_Multiplier_100000 = 8,
};

enum E_A5_09_0B_Unit
{
   kA5_09_0B_uSvh = 0,
   kA5_09_0B_cpm = 1,
   kA5_09_0B_BqL = 2,
   kA5_09_0B_BqKg = 3
};

CProfile_A5_09_0B::CProfile_A5_09_0B(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_supplyVoltage(boost::make_shared<yApi::historization::CVoltage>("supplyVoltage")),
     m_sieverPerSecondRadioActivity(
        boost::make_shared<specificHistorizers::CSieverPerSecondRadioActivity>("radioActivity(Sv/s)")),
     m_countPerSecondRadioActivity(
        boost::make_shared<specificHistorizers::CCountPerSecondRadioActivity>("radioActivity(cps)")),
     m_volumicRadioActivity(boost::make_shared<specificHistorizers::CVolumicRadioActivity>("radioActivity(Bq/m3)")),
     m_massRadioActivity(boost::make_shared<specificHistorizers::CMassRadioActivity>("radioActivity(Bq/g)")),
     m_historizers({
        m_supplyVoltage,
        m_sieverPerSecondRadioActivity,
        m_countPerSecondRadioActivity,
        m_volumicRadioActivity,
        m_massRadioActivity
     })
{
}

const std::string& CProfile_A5_09_0B::profile() const
{
   static const std::string Profile("A5-09-0B");
   return Profile;
}

const std::string& CProfile_A5_09_0B::title() const
{
   static const std::string Title("Gas sensor - Radioactivity sensor");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_0B::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_09_0B::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_09_0B::states(
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

   if (bitset_extract(data, 31, 1))
   {
      // Supply voltage sensor available
      m_supplyVoltage->set(static_cast<double>(bitset_extract(data, 0, 4)) * 3.0 / 15.0 + 2.0);
      historizers.push_back(m_supplyVoltage);
   }

   auto radiationLevel = static_cast<double>(bitset_extract(data, 8, 16));
   const auto scaleMultiplier = static_cast<E_A5_09_0B_ScaleMultiplier>(bitset_extract(data, 24, 4));
   const auto unit = static_cast<E_A5_09_0B_Unit>(bitset_extract(data, 29, 2));

   switch (scaleMultiplier)
   {
   case kA5_09_0B_Multiplier_0_001: radiationLevel *= 0.001;
      break;
   case kA5_09_0B_Multiplier_0_01: radiationLevel *= 0.01;
      break;
   case kA5_09_0B_Multiplier_0_1: radiationLevel *= 0.1;
      break;
   case kA5_09_0B_Multiplier_1:
      break;
   case kA5_09_0B_Multiplier_10: radiationLevel *= 10.0;
      break;
   case kA5_09_0B_Multiplier_100: radiationLevel *= 100.0;
      break;
   case kA5_09_0B_Multiplier_1000: radiationLevel *= 1000.0;
      break;
   case kA5_09_0B_Multiplier_10000: radiationLevel *= 10000.0;
      break;
   case kA5_09_0B_Multiplier_100000: radiationLevel *= 100000.0;
      break;
   default:
      YADOMS_LOG(error) << "A5-09-0B profile : received unknown multiplier" << scaleMultiplier;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   switch (unit)
   {
   case kA5_09_0B_uSvh:
      // radiationLevel is given in µSv/h, expected in Sv/s
      m_sieverPerSecondRadioActivity->set(radiationLevel / 1000000.0 / 3600.0);
      historizers.push_back(m_sieverPerSecondRadioActivity);
      break;
   case kA5_09_0B_cpm:
      // radiationLevel is given in cpm, expected in cps
      m_countPerSecondRadioActivity->set(radiationLevel / 60.0);
      historizers.push_back(m_countPerSecondRadioActivity);
      break;
   case kA5_09_0B_BqL:
      // radiationLevel is given in Bq/L, expected in Bq/m3
      m_volumicRadioActivity->set(radiationLevel / 1000.0);
      historizers.push_back(m_volumicRadioActivity);
      break;
   case kA5_09_0B_BqKg:
      // radiationLevel is given in Bq/kg, expected in Bq/g
      m_massRadioActivity->set(radiationLevel / 1000.0);
      historizers.push_back(m_massRadioActivity);
      break;
   }

   return historizers;
}

void CProfile_A5_09_0B::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_09_0B::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
