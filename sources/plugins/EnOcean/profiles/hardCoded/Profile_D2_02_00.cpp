#include "stdafx.h"
#include "Profile_D2_02_00.h"

#include <utility>
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include "message/MessageHelpers.h"


CProfile_D2_02_00::CProfile_D2_02_00(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(std::move(deviceId)),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature",
                                                                         yApi::EKeywordAccessMode::kGet)),
     m_illumination(boost::make_shared<yApi::historization::CIllumination>("Illumination",
                                                                           yApi::EKeywordAccessMode::kGet)),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy",
                                                                  yApi::EKeywordAccessMode::kGet)),
     m_smoke(boost::make_shared<yApi::historization::CSwitch>("Smoke",
                                                              yApi::EKeywordAccessMode::kGet)),
     m_historizers({
        m_temperature, m_illumination, m_occupancy, m_smoke
     })
{
}

const std::string& CProfile_D2_02_00::profile() const
{
   static const std::string Profile("D2-02-00");
   return Profile;
}

const std::string& CProfile_D2_02_00::title() const
{
   static const std::string Title(R"(Sensors for Temperature, Illumination, Occupancy And Smoke)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_02_00::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_02_00::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   sendSensorMeasurementQuery(EMeasurementType::kTemperature, senderId, messageHandler);

   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   sendSensorMeasurementQuery(EMeasurementType::kIllumination, senderId, messageHandler);

   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   sendSensorMeasurementQuery(EMeasurementType::kOccupancy, senderId, messageHandler);

   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   sendSensorMeasurementQuery(EMeasurementType::kSmoke, senderId, messageHandler);
}

void CProfile_D2_02_00::sendSensorMeasurementQuery(EMeasurementType query,
                                                   const std::string& senderId,
                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   boost::dynamic_bitset<> userData(2 * 8);
   bitset_insert(userData, 4, 4, static_cast<int>(EMsgId::kSensorMeasurementQuery));
   bitset_insert(userData, 8, 3, static_cast<int>(query));

   message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
                                         messageHandler,
                                         senderId,
                                         m_deviceId,
                                         userData,
                                         "Sensor Measurement Query");
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_02_00::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   if (static_cast<EMsgId>(bitset_extract(data, 4, 4)) != EMsgId::kSensorMeasurement)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Command ID=" << bitset_extract(data, 4, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto rawValue = bitset_extract(data, 16, 16);

   switch (static_cast<EMeasurementType>(bitset_extract(data, 8, 3)))
   {
   case EMeasurementType::kTemperature:
      m_temperature->set(static_cast<double>(rawValue) * 160.0 / 65535.0 - 40.0);
      historizers.emplace_back(m_temperature);
      break;
   case EMeasurementType::kIllumination:
      m_illumination->set(static_cast<double>(rawValue) * 2047.0 / 65535.0);
      historizers.emplace_back(m_illumination);
      break;
   case EMeasurementType::kOccupancy:
      m_occupancy->set(rawValue ? true : false);
      historizers.emplace_back(m_occupancy);
      break;
   case EMeasurementType::kSmoke:
      m_smoke->set(rawValue ? true : false);
      historizers.emplace_back(m_smoke);
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Measurement Type=" << bitset_extract(data, 8, 3);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return historizers;
}

void CProfile_D2_02_00::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_02_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto deltaTemperatureToReport = deviceConfiguration.get<double>("deltaTemperatureToReport");
   const auto deltaIlluminationToReport = deviceConfiguration.get<double>("deltaIlluminationToReport");
   const auto maxMessagesInterval = deviceConfiguration.get<unsigned int>("maxMessagesInterval");
   const auto minMessagesInterval = deviceConfiguration.get<unsigned int>("minMessagesInterval");

   boost::dynamic_bitset<> userData(6 * 8);
   bitset_insert(userData, 4, 4, static_cast<int>(EMsgId::kActuatorSetMeasurement));
   bitset_insert(userData, 8, 1, 1);

   // Delta temperature report is expressed as binary value as 0..4095.
   // Temperature is expressed as 0..65535 covers -40..+120°.
   // So delta temperature report is 0..10 °C
   const auto binaryDeltaTemperatureToReport = static_cast<unsigned int>(deltaTemperatureToReport * 4095.0 / 10.0);

   bitset_insert(userData, 16, 4, binaryDeltaTemperatureToReport & 0x0F);
   bitset_insert(userData, 24, 8, binaryDeltaTemperatureToReport >> 4 & 0xFF);
   bitset_insert(userData, 32, 8, maxMessagesInterval / 10);
   bitset_insert(userData, 40, 8, minMessagesInterval);

   message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
                                         messageHandler,
                                         senderId,
                                         m_deviceId,
                                         userData,
                                         "Actuator Set Measurement - temperature");

   // Delta illumination report is expressed as binary value as 0..4095.
   // Illumination is expressed as 0..65535 covers 0..2047lx.
   // So delta illumination report is 0..128 lx
   const auto binaryDeltaIlluminationToReport = static_cast<unsigned int>(deltaIlluminationToReport * 4095.0 / 128.0);

   bitset_insert(userData, 16, 4, binaryDeltaIlluminationToReport & 0x0F);
   bitset_insert(userData, 24, 8, binaryDeltaIlluminationToReport >> 4 & 0xFF);

   message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
                                         messageHandler,
                                         senderId,
                                         m_deviceId,
                                         userData,
                                         "Actuator Set Measurement - illumination");
}
