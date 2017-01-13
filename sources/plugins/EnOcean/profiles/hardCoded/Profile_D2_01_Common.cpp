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


void CProfile_D2_01_Common::sendActuatorSetLocalCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                        const std::string& senderId,
                                                        const std::string& targetId,
                                                        bool localControl,
                                                        bool taughtInAllDevices,
                                                        bool userInterfaceDayMode,
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
   bitset_insert(data, 11, 5, 0x1E); // Use all output channels supported by the device
   bitset_insert(data, 16, 4, static_cast<unsigned int>(lround(dimTimer2 / 0.5)));
   bitset_insert(data, 20, 4, static_cast<unsigned int>(lround(dimTimer3 / 0.5)));
   bitset_insert(data, 24, !userInterfaceDayMode);
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
      YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : no answer to Actuator Set Local command" ;

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : Actuator Set Local command returns " << response->returnCode() ;
}

void CProfile_D2_01_Common::sendActuatorSetExternalInterfaceSettingsCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                                            const std::string& senderId,
                                                                            const std::string& targetId,
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
   bitset_insert(data, 11, 5, 0x1E); // Use all output channels supported by the device
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
      YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : no answer to Actuator Set External Interface Settings command" ;

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : Actuator Set External Interface Settings command returns " << response->returnCode() ;
}

void CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                              const std::string& senderId,
                                                              const std::string& targetId,
                                                              bool powerMeasurement,
                                                              bool outputChannels,
                                                              double minEnergyMeasureRefreshTime,
                                                              double maxEnergyMeasureRefreshTime)
{
   message::CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
                                          senderId,
                                          targetId,
                                          0);
   boost::dynamic_bitset<> data(7 * 8);

   bitset_insert(data, 4, 4, kActuatorSetMeasurement);
   bitset_insert(data, 8, 1, 1); // support only auto reporting
   bitset_insert(data, 9, 1, 1); // reset measurement not (yet ?) available
   bitset_insert(data, 10, 1, powerMeasurement);
   bitset_insert(data, 11, 5, outputChannels ? 0x1E : 0x1F); // 0x1E : all output channels, 0x1F : input channel
   bitset_insert(data, 16, 4, 0); // No measurement delta
   bitset_insert(data, 21, 3, powerMeasurement ? kPowerW : kEnergyWh); // Hard-coded for now
   bitset_insert(data, 24, 8, 0); // No measurement delta
   bitset_insert(data, 32, 8, static_cast<unsigned char>(minEnergyMeasureRefreshTime / 10));
   bitset_insert(data, 40, 8, static_cast<unsigned char>(maxEnergyMeasureRefreshTime));

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
      YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : no answer to Actuator Set Measurement command" ;

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send configuration to " << targetId << " : Actuator Set Measurement command returns " << response->returnCode() ;
}
