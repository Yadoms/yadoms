#include "stdafx.h"
#include "Profile_A5_12_10.h"
#include "Profile_A5_12_Common.h"
#include <profiles/eep.h>
#include <profiles/bitsetHelpers.hpp>

CProfile_A5_12_10::CProfile_A5_12_10(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
{
   for (auto indexChannel = 0; indexChannel < NbChannels; ++indexChannel)
   {
      const auto strChannel = std::to_string(indexChannel);
      m_current[indexChannel] = boost::make_shared<yApi::historization::CCurrent>("Current (" + strChannel + ")");
      m_historizers.emplace_back(m_current[indexChannel]);
      m_electricLoad[indexChannel] = boost::make_shared<yApi::historization::CElectricLoad>("Load (" + strChannel + ")");
      m_historizers.emplace_back(m_electricLoad[indexChannel]);
   }
}

const std::string& CProfile_A5_12_10::profile() const
{
   static const std::string Profile("A5-12-10");
   return Profile;
}

const std::string& CProfile_A5_12_10::title() const
{
   static const std::string Title(R"(Automated Meter Reading - Current meter 16 channels)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_10::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_12_10::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_12_10::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Some devices supports several RORG telegrams, ignore non-4BS telegrams
   if (rorg != CRorgs::ERorgIds::k4BS_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   const auto meterReading = static_cast<int>(bitset_extract(data, 0, 24));
   const auto channel = static_cast<int>(bitset_extract(data, 24, 4));
   const auto dataType = static_cast<CProfile_A5_12_Common::E_A5_12_DataType> (bitset_extract(data, 29, 1));
   const auto divisor = static_cast<CProfile_A5_12_Common::E_A5_12_Divisor> (bitset_extract(data, 30, 2));

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   switch (dataType)
   {
   case CProfile_A5_12_Common::E_A5_12_DataType::kCumulativeValue:
      m_electricLoad[channel]->set(CProfile_A5_12_Common::applyDivisorInDouble(meterReading, divisor));
      historizers.emplace_back(m_electricLoad[channel]);
      break;
   case CProfile_A5_12_Common::E_A5_12_DataType::kCurrentValue:
      m_current[channel]->set(1000.0 * CProfile_A5_12_Common::applyDivisorInDouble(meterReading, divisor)); // Provided in mA
      historizers.emplace_back(m_current[channel]);
      break;
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return historizers;
}

void CProfile_A5_12_10::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

void CProfile_A5_12_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
}
