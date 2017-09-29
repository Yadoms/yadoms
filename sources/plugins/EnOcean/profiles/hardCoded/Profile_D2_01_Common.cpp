#include "stdafx.h"
#include "Profile_D2_01_Common.h"
#include "../../message/ResponseReceivedMessage.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include <shared/Log.h>

DECLARE_ENUM_IMPLEMENTATION_NESTED(CProfile_D2_01_Common::EDefaultState, EDefaultState,
   ((off))
   ((on))
   ((previousState))
   ((notUsed))
);

DECLARE_ENUM_IMPLEMENTATION_NESTED(CProfile_D2_01_Common::EConnectedSwitchsType, EConnectedSwitchsType,
   ((switch))
   ((pushButton))
   ((autodetection))
);


void CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(boost::shared_ptr<IMessageHandler> messageHandler,
                                                                  const std::string& senderId,
                                                                  const std::string& targetId,
                                                                  EOutputChannel outputChannel,
                                                                  bool state)
{
   boost::dynamic_bitset<> userData(3 * 8);
   bitset_insert(userData, 4, 4, kActuatorSetOutput);
   bitset_insert(userData, 11, 5, outputChannel);
   bitset_insert(userData, 17, 7, state ? 100 : 0);

   sendMessage(messageHandler,
               senderId,
               targetId,
               userData,
               "Actuator Set Output");
}

void CProfile_D2_01_Common::sendActuatorSetOutputCommandDimming(boost::shared_ptr<IMessageHandler> messageHandler,
                                                                const std::string& senderId,
                                                                const std::string& targetId,
                                                                EOutputChannel outputChannel,
                                                                const specificHistorizers::EDimmerMode& mode,
                                                                unsigned int dimValue)
{
   boost::dynamic_bitset<> userData(3 * 8);
   bitset_insert(userData, 4, 4, kActuatorSetOutput);
   E_D2_01_DimMode dimMode;
   switch (mode)
   {
   case specificHistorizers::EDimmerMode::kSwitchToValueValue: dimMode = kSwitchToValue;
      break;
   case specificHistorizers::EDimmerMode::kDimToValueWithTimer1Value: dimMode = kDimToValueWithTimer1;
      break;
   case specificHistorizers::EDimmerMode::kDimToValueWithTimer2Value: dimMode = kDimToValueWithTimer2;
      break;
   case specificHistorizers::EDimmerMode::kDimToValueWithTimer3Value: dimMode = kDimToValueWithTimer3;
      break;
   case specificHistorizers::EDimmerMode::kStopDimmingValue: dimMode = kStopDimming;
      break;
   default:
      {
         std::ostringstream oss;
         oss << "Device " << targetId << " : send Actuator Set Pilot Wire Mode command with unsupported value " << mode;
         YADOMS_LOG(information) << oss.str();
         throw std::logic_error(oss.str());
      }
   }
   bitset_insert(userData, 8, 3, dimMode);
   bitset_insert(userData, 11, 5, outputChannel);
   bitset_insert(userData, 17, 7, dimValue);

   sendMessage(messageHandler,
               senderId,
               targetId,
               userData,
               "Actuator Set Output");
}

void CProfile_D2_01_Common::sendActuatorSetLocalCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                        const std::string& senderId,
                                                        const std::string& targetId,
                                                        EOutputChannel outputChannel,
                                                        bool localControl,
                                                        bool taughtInAllDevices,
                                                        bool userInterfaceDayMode,
                                                        bool powerFailureDetection,
                                                        const EDefaultState& defaultState,
                                                        double dimTimer1,
                                                        double dimTimer2,
                                                        double dimTimer3)
{
   message::CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
                                          senderId,
                                          targetId,
                                          0);
   boost::dynamic_bitset<> data(4 * 8);

   bitset_insert(data, 4, 4, kActuatorSetLocal);
   bitset_insert(data, 0, !taughtInAllDevices);
   bitset_insert(data, 10, localControl);
   bitset_insert(data, 11, 5, outputChannel);
   bitset_insert(data, 16, 4, static_cast<unsigned int>(lround(dimTimer2 / 0.5)));
   bitset_insert(data, 20, 4, static_cast<unsigned int>(lround(dimTimer3 / 0.5)));
   bitset_insert(data, 24, !userInterfaceDayMode);
   bitset_insert(data, 25, powerFailureDetection);
   bitset_insert(data, 26, 2, defaultState);
   bitset_insert(data, 28, 4, static_cast<unsigned int>(lround(dimTimer1 / 0.5)));

   command.userData(bitset_to_bytes(data));

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
   YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : no answer to Actuator Set Local command";

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
   YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : Actuator Set Local command returns " << response->returnCode();
}

void CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                              const std::string& senderId,
                                                              const std::string& targetId,
                                                              EOutputChannel outputChannel,
                                                              bool powerMeasurement,
                                                              double minEnergyMeasureRefreshTime,
                                                              double maxEnergyMeasureRefreshTime)
{
   message::CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
                                          senderId,
                                          targetId,
                                          0);
   boost::dynamic_bitset<> data(7 * 8);

   bitset_insert(data, 4, 4, kActuatorSetMeasurement);
   bitset_insert(data, 8, true); // Report on query + auto reporting
   bitset_insert(data, 9, false); // reset measurement not (yet ?) available
   bitset_insert(data, 10, powerMeasurement);
   bitset_insert(data, 11, 5, outputChannel); // Specific channel, or 0x1E for all output channels, or 0x1F for input channel
   bitset_insert(data, 16, 4, 0); // No measurement delta
   bitset_insert(data, 21, 3, powerMeasurement ? kPowerW : kEnergyWh); // Hard-coded for now
   bitset_insert(data, 24, 8, 0); // No measurement delta
   bitset_insert(data, 32, 8, static_cast<unsigned char>(maxEnergyMeasureRefreshTime / 10.0));
   bitset_insert(data, 40, 8, static_cast<unsigned char>(minEnergyMeasureRefreshTime));

   command.userData(bitset_to_bytes(data));

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
   YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : no answer to Actuator Set Measurement command";

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
   YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : Actuator Set Measurement command returns " << response->returnCode();
}

void CProfile_D2_01_Common::sendActuatorMeasurementQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                                         const std::string& senderId,
                                                         const std::string& targetId,
                                                         EOutputChannel outputChannel)
{
   boost::dynamic_bitset<> userData(2 * 8);
   bitset_insert(userData, 4, 4, kActuatorMeasurementQuery);
   bitset_insert(userData, 10, 1, kQueryEnergy);
   bitset_insert(userData, 11, 5, outputChannel);

   sendMessage(messageHandler,
               senderId,
               targetId,
               userData,
               "Actuator Measurement Query");
}

void CProfile_D2_01_Common::sendActuatorSetPilotWireModeCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                                const std::string& senderId,
                                                                const std::string& targetId,
                                                                const specificHistorizers::EPilotWire& mode)
{
   boost::dynamic_bitset<> userData(3 * 8);

   bitset_insert(userData, 4, 4, kActuatorSetPilotWireMode);
   EPilotWireMode pilotWireMode;
   switch (mode)
   {
   case specificHistorizers::EPilotWire::kOffValue: pilotWireMode = kOff;
      break;
   case specificHistorizers::EPilotWire::kComfortValue: pilotWireMode = kComfort;
      break;
   case specificHistorizers::EPilotWire::kComfort2Value: pilotWireMode = kComfort_1;
      break;
   case specificHistorizers::EPilotWire::kComfort3Value: pilotWireMode = kComfort_2;
      break;
   case specificHistorizers::EPilotWire::kEcoValue: pilotWireMode = kEco;
      break;
   case specificHistorizers::EPilotWire::kAntiFreezeValue: pilotWireMode = kAntiFreeze;
      break;
   default:
      {
         std::ostringstream oss;
         oss << "Device " << targetId << " : send Actuator Set Pilot Wire Mode command with unsupported value " << mode;
         YADOMS_LOG(information) << oss.str();
         throw std::logic_error(oss.str());
      }
   }
   bitset_insert(userData, 13, 3, pilotWireMode);

   sendMessage(messageHandler,
               senderId,
               targetId,
               userData,
               "Actuator Set Pilot Wire Mode");
}

void CProfile_D2_01_Common::sendActuatorSetExternalInterfaceSettingsCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                                            const std::string& senderId,
                                                                            const std::string& targetId,
                                                                            EOutputChannel outputChannel,
                                                                            const EConnectedSwitchsType& connectedSwitchsType,
                                                                            double autoOffTimerSeconds,
                                                                            double delayRadioOffTimerSeconds,
                                                                            bool switchingStateToggle)
{
   message::CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
                                          senderId,
                                          targetId,
                                          0);
   boost::dynamic_bitset<> data(7 * 8);

   bitset_insert(data, 4, 4, kActuatorSetExternalInterfaceSettings);
   bitset_insert(data, 11, 5, outputChannel);
   bitset_insert(data, 16, 16, static_cast<unsigned int>(autoOffTimerSeconds * 10));
   bitset_insert(data, 32, 16, static_cast<unsigned int>(delayRadioOffTimerSeconds * 10));
   bitset_insert(data, 48, 2, connectedSwitchsType);
   bitset_insert(data, 50, !switchingStateToggle);

   command.userData(bitset_to_bytes(data));

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
   YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : no answer to Actuator Set External Interface Settings command";

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
   YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : Actuator Set External Interface Settings command returns " << response->returnCode();
}

void CProfile_D2_01_Common::sendMessage(boost::shared_ptr<IMessageHandler> messageHandler,
                                        const std::string& senderId,
                                        const std::string& targetId,
                                        const boost::dynamic_bitset<>& userData,
                                        const std::string& commandName)
{
   message::CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
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
   YADOMS_LOG(error) << "Fail to send message to " << targetId << " : no answer to \"" << commandName << "\"";

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
   YADOMS_LOG(error) << "Fail to send message to " << targetId << " : \"" << commandName << "\" returns " << response->returnCode();
}

//TODO refactoring D2-01-XX, RAF :
// - over current
// - mesure d'énergy/power fonctionnelle pour tous les profils qui sont censés le supporter
// - voir ce que c'est que le "Measurement Auto Scaling"
// - voir si on peut factoriser la fonction state
// - Devices à 2 canaux : distinguer la conf pour chaque canal
