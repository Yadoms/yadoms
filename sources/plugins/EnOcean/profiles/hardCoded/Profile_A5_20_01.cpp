#include "stdafx.h"
#include "Profile_A5_20_01.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include "message/RadioErp1SendMessage.h"
#include "message/ResponseReceivedMessage.h"


CProfile_A5_20_01::CProfile_A5_20_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_currentValue(
        boost::make_shared<yApi::historization::CDimmable>("Current Value", yApi::EKeywordAccessMode::kGet)),
     m_serviceOn(boost::make_shared<yApi::historization::CSwitch>("Service On", yApi::EKeywordAccessMode::kGet)),
     m_energyInputEnable(
        boost::make_shared<yApi::historization::CSwitch>("Energy Input Enable", yApi::EKeywordAccessMode::kGet)),
     m_energyStorageCharged(
        boost::make_shared<yApi::historization::CSwitch>("Energy Storage Charged", yApi::EKeywordAccessMode::kGet)),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery", yApi::EKeywordAccessMode::kGet)),
     m_coverOpen(boost::make_shared<yApi::historization::CSwitch>("Cover Open", yApi::EKeywordAccessMode::kGet)),
     m_temperatureFailure(
        boost::make_shared<yApi::historization::CSwitch>("Temperature Failure", yApi::EKeywordAccessMode::kGet)),
     m_windowOpen(boost::make_shared<yApi::historization::CSwitch>("Window Open", yApi::EKeywordAccessMode::kGet)),
     m_actuatorObstructed(
        boost::make_shared<yApi::historization::CSwitch>("Actuator Obstructed", yApi::EKeywordAccessMode::kGet)),
     m_temperature(
        boost::make_shared<yApi::historization::CTemperature>("Temperature", yApi::EKeywordAccessMode::kGet)),
     m_valvePosition(
        boost::make_shared<yApi::historization::CDimmable>("Valve Position")),
     m_temperatureSetPoint(
        boost::make_shared<yApi::historization::CTemperature>("Temperature", yApi::EKeywordAccessMode::kGetSet)),
     m_currentTemperatureFromExternalSensor(
        boost::make_shared<yApi::historization::CTemperature>("Temperature", yApi::EKeywordAccessMode::kGetSet)),
     m_runInitSequence(
        boost::make_shared<yApi::historization::CEvent>("Run init sequence", yApi::EKeywordAccessMode::kGetSet)),
     m_liftSet(
        boost::make_shared<yApi::historization::CEvent>("Lift set", yApi::EKeywordAccessMode::kGetSet)),
     m_summerMode(
        boost::make_shared<yApi::historization::CSwitch>("Summer mode", yApi::EKeywordAccessMode::kGet)),
     m_setPointInverse(
        boost::make_shared<yApi::historization::CSwitch>("Set point inverse", yApi::EKeywordAccessMode::kGet)),
     m_historizers({
        m_currentValue, m_serviceOn, m_energyInputEnable, m_energyStorageCharged, m_battery, m_coverOpen,
        m_temperatureFailure, m_windowOpen, m_actuatorObstructed, m_temperature, m_valvePosition, m_temperatureSetPoint,
        m_currentTemperatureFromExternalSensor, m_runInitSequence, m_liftSet, m_summerMode, m_setPointInverse
     })
{
}

const std::string& CProfile_A5_20_01::profile() const
{
   static const std::string Profile("A5-20-01");
   return Profile;
}

const std::string& CProfile_A5_20_01::title() const
{
   static const std::string Title(
      "HVAC Components - Battery Powered Actuator (BI-DIR)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_20_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_20_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_20_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_currentValue->set(static_cast<int>(bitset_extract(data, 0, 8)));
   m_serviceOn->set(bitset_extract(data, 8, 1) ? true : false);
   m_energyInputEnable->set(bitset_extract(data, 9, 1) ? true : false);
   m_energyStorageCharged->set(bitset_extract(data, 10, 1) ? true : false);
   m_battery->set(bitset_extract(data, 11, 1) ? 0 : 100);
   m_coverOpen->set(bitset_extract(data, 12, 1) ? true : false);
   m_temperatureFailure->set(bitset_extract(data, 13, 1) ? true : false);
   m_windowOpen->set(bitset_extract(data, 14, 1) ? true : false);
   m_actuatorObstructed->set(bitset_extract(data, 15, 1) ? true : false);
   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 40.0 / 255.0);

   return m_historizers;
}

void CProfile_A5_20_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Set internal state
   if (keyword == m_valvePosition->getKeyword())
      m_setPointModeIsTemperature = false;
   if (keyword == m_temperatureSetPoint->getKeyword())
      m_setPointModeIsTemperature = true;

   if (!m_setPointModeIsTemperature.has_value())
   {
      YADOMS_LOG(error) << "Set point mode still not defined, can not send message or wrong data will be sent to " <<
         senderId;
      return;
   }

   if (keyword == m_valvePosition->getKeyword())
      m_valvePosition->setCommand(commandBody);
   if (keyword == m_temperatureSetPoint->getKeyword())
      m_temperatureSetPoint->setCommand(commandBody);
   if (keyword == m_currentTemperatureFromExternalSensor->getKeyword())
      m_currentTemperatureFromExternalSensor->setCommand(commandBody);
   if (keyword == m_summerMode->getKeyword())
      m_summerMode->setCommand(commandBody);
   if (keyword == m_setPointInverse->getKeyword())
      m_setPointInverse->setCommand(commandBody);


   // Send message
   boost::dynamic_bitset<> userData(4 * 8);

   bitset_insert(userData, 0, 8, *m_setPointModeIsTemperature
                                    ? static_cast<unsigned int>(m_temperatureSetPoint->get() * 255.0 / 40.0)
                                    : m_valvePosition->get());
   bitset_insert(userData, 21, 1, *m_setPointModeIsTemperature);
   bitset_insert(userData, 8, 8,
                 static_cast<unsigned int>(m_currentTemperatureFromExternalSensor->get() * 255.0 / 40.0));
   bitset_insert(userData, 23, 1, false);
   bitset_insert(userData, 16, 1, keyword == m_runInitSequence->getKeyword());
   bitset_insert(userData, 17, 1, keyword == m_liftSet->getKeyword());
   bitset_insert(userData, 20, 1, m_summerMode->get());
   bitset_insert(userData, 22, 1, m_setPointInverse->get());

   sendMessage(messageHandler,
               senderId,
               m_deviceId,
               userData);
}

void CProfile_A5_20_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}


void CProfile_A5_20_01::sendMessage(boost::shared_ptr<IMessageHandler> messageHandler,
                                    const std::string& senderId,
                                    const std::string& targetId,
                                    const boost::dynamic_bitset<>& userData)
{
   message::CRadioErp1SendMessage command(CRorgs::k4BS_Telegram,
                                          senderId,
                                          targetId,
                                          0);

   command.userData(bitset_to_bytes(userData));

   boost::shared_ptr<const message::CEsp3ReceivedPacket> answer;
   if (!messageHandler->send(command,
                             [](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                             {
                                return esp3Packet->header().packetType() == message::RESPONSE;
                             },
                             [&](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                             {
                                answer = esp3Packet;
                             }))
      throw std::runtime_error(
         (boost::format("Fail to send message to %1% : no answer") % targetId).str());

   const auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send message to " << targetId << " : returns " << response->returnCode();
}
