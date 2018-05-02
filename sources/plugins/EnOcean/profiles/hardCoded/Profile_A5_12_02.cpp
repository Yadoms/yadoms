#include "stdafx.h"
#include "Profile_A5_12_02.h"
#include "Profile_A5_12_Common.h"
#include "profiles/eep.h"
#include "profiles/bitsetHelpers.hpp"

CProfile_A5_12_02::CProfile_A5_12_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId)
{
   for (auto indexChannel = 0; indexChannel < NB_TARIFFS; ++indexChannel)
   {
      const auto strTariff = std::to_string(indexChannel);
      m_volume[indexChannel] = boost::make_shared<yApi::historization::CVolume>("Volume (" + strTariff + ")");
      m_historizers.push_back(m_volume[indexChannel]);
      m_debit[indexChannel] = boost::make_shared<yApi::historization::CDebit>("Debit (" + strTariff + ")");
      m_historizers.push_back(m_debit[indexChannel]);
   }
}

CProfile_A5_12_02::~CProfile_A5_12_02()
{
}

const std::string& CProfile_A5_12_02::profile() const
{
   static const std::string profile("A5-12-02");
   return profile;
}

const std::string& CProfile_A5_12_02::title() const
{
   static const std::string title("Automated Meter Reading - Gas");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_12_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_02::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   const int meterReading = bitset_extract(data, 0, 24);
   const int channel = bitset_extract(data, 24, 4);
   const auto dataType = static_cast<CProfile_A5_12_Common::E_A5_12_DataType> (bitset_extract(data, 29, 1));
   const auto divisor = static_cast<CProfile_A5_12_Common::E_A5_12_Divisor> (bitset_extract(data, 30, 2));

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   switch (dataType)
   {
   case CProfile_A5_12_Common::kCumulativeValue:
      m_volume[channel]->set(CProfile_A5_12_Common::applyDivisorInDouble(meterReading, divisor));
      historizers.push_back(m_volume[channel]);
   case CProfile_A5_12_Common::kCurrentValue:
      m_debit[channel]->set(CProfile_A5_12_Common::applyDivisorInDouble(meterReading, divisor) / 1000); // Provided in liter/s
      historizers.push_back(m_debit[channel]);
      break;
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return historizers;
}

void CProfile_A5_12_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_12_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
