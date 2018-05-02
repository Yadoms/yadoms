#include "stdafx.h"
#include "Profile_A5_12_01.h"
#include "Profile_A5_12_Common.h"
#include "profiles/eep.h"
#include "profiles/bitsetHelpers.hpp"

CProfile_A5_12_01::CProfile_A5_12_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId)
{
   for (auto indexTariffInfo = 0; indexTariffInfo < NB_TARIFF_INFO; ++indexTariffInfo)
   {
      const auto strTariff = std::to_string(indexTariffInfo);
      m_energy[indexTariffInfo] = boost::make_shared<yApi::historization::CEnergy>("Energy (" + strTariff + ")");
      m_historizers.push_back(m_energy[indexTariffInfo]);
      m_instantPower[indexTariffInfo] = boost::make_shared<yApi::historization::CPower>("InstantPower (" + strTariff + ")");
      m_historizers.push_back(m_instantPower[indexTariffInfo]);
   }
}

CProfile_A5_12_01::~CProfile_A5_12_01()
{
}

const std::string& CProfile_A5_12_01::profile() const
{
   static const std::string profile("A5-12-01");
   return profile;
}

const std::string& CProfile_A5_12_01::title() const
{
   static const std::string title("Automated Meter Reading - Electricity");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_12_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_01::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   const int meterReading = bitset_extract(data, 0, 24);
   const int tarifInfo = bitset_extract(data, 24, 4);
   const auto dataType = static_cast<CProfile_A5_12_Common::E_A5_12_DataType> (bitset_extract(data, 29, 1));
   const auto divisor = static_cast<CProfile_A5_12_Common::E_A5_12_Divisor> (bitset_extract(data, 30, 2));

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   switch (dataType)
   {
   case CProfile_A5_12_Common::kCumulativeValue:
      m_energy[tarifInfo]->set(1000 * CProfile_A5_12_Common::applyDivisorInInt(meterReading, divisor)); // Energy is in Wh when provided in kWh
      historizers.push_back(m_energy[tarifInfo]);
   case CProfile_A5_12_Common::kCurrentValue:
      m_instantPower[tarifInfo]->set(CProfile_A5_12_Common::applyDivisorInDouble(meterReading, divisor));
      historizers.push_back(m_instantPower[tarifInfo]);
      break;
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return historizers;
}

void CProfile_A5_12_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_12_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
