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
   ((externalSwitch))
   ((externalPushButton))
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
   boost::dynamic_bitset<> data(4 * 8);

   bitset_insert(data, 4, 4, kActuatorSetLocal);
   bitset_insert(data, 0, taughtInAllDevices);
   bitset_insert(data, 8, true); // Over-current shut-down : automatic restart
   bitset_insert(data, 10, localControl);
   bitset_insert(data, 11, 5, outputChannel);
   bitset_insert(data, 16, 4, static_cast<unsigned int>(lround(dimTimer2 / 0.5)));
   bitset_insert(data, 20, 4, static_cast<unsigned int>(lround(dimTimer3 / 0.5)));
   bitset_insert(data, 24, !userInterfaceDayMode);
   bitset_insert(data, 25, powerFailureDetection);
   bitset_insert(data, 26, 2, defaultState);
   bitset_insert(data, 28, 4, static_cast<unsigned int>(lround(dimTimer1 / 0.5)));

   sendMessage(messageHandler,
               senderId,
               targetId,
               data,
               "Actuator Set Local");
}

void CProfile_D2_01_Common::sendActuatorStatusQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                                    const std::string& senderId,
                                                    const std::string& targetId,
                                                    EOutputChannel outputChannel)
{
   boost::dynamic_bitset<> data(2 * 8);

   bitset_insert(data, 4, 4, kActuatorStatusQuery);
   bitset_insert(data, 11, 5, outputChannel);

   sendMessage(messageHandler,
               senderId,
               targetId,
               data,
               "Actuator Status Query");
}


const boost::shared_ptr<yApi::historization::CSwitch> CProfile_D2_01_Common::noChannel1 = boost::shared_ptr<yApi::historization::CSwitch>();
const boost::shared_ptr<yApi::historization::CSwitch> CProfile_D2_01_Common::noChannel2 = boost::shared_ptr<yApi::historization::CSwitch>();
const boost::shared_ptr<yApi::historization::CDimmable> CProfile_D2_01_Common::noDimmable = boost::shared_ptr<yApi::historization::CDimmable>();
const boost::shared_ptr<yApi::historization::CSwitch> CProfile_D2_01_Common::noPowerFailure = boost::shared_ptr<yApi::historization::CSwitch>();
const boost::shared_ptr<yApi::historization::CSwitch> CProfile_D2_01_Common::noOverCurrent = boost::shared_ptr<yApi::historization::CSwitch>();

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_Common::extractActuatorStatusResponse(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   boost::shared_ptr<yApi::historization::CSwitch> channel1,
   boost::shared_ptr<yApi::historization::CDimmable> dimmer,
   boost::shared_ptr<yApi::historization::CSwitch>
   powerFailure,
   boost::shared_ptr<yApi::historization::CSwitch> overCurrent)
{
   // Some devices supports several RORG telegrams, ignore non-VLD telegrams
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   if (bitset_extract(data, 4, 4) != kActuatorStatusResponse)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizers
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto ioChannel = bitset_extract(data, 11, 5);
   const int dimValue = bitset_extract(data, 17, 7);
   const auto state = dimValue == 0 ? false : true;
   const auto overCurrentState = bitset_extract(data, 8, 1) != 0;

   // Sometimes ioChannel is not well set by device (ex NODON ASP-2-1-00 set ioChannel to 1 instead of 0),
   // so ignore ioChannel value (juste verify that is not input channel)
   if (ioChannel == kInputChannel)
      YADOMS_LOG(warning) << "ActuatorStatusResponse : received unsupported ioChannel value " << ioChannel;
   else
   {
      if (!!channel1)
      {
         channel1->set(state);
         historizers.push_back(channel1);
      }
      else if (!!dimmer)
      {
         dimmer->set(dimValue);
         historizers.push_back(dimmer);
      }
      else
      {
         YADOMS_LOG(warning) << "ActuatorStatusResponse : received unsupported ioChannel value " << ioChannel;
      }
   }

   if (!!overCurrent)
   {
      overCurrent->set(overCurrentState);
      historizers.push_back(overCurrent);
   }

   const auto powerFailureSupported = bitset_extract(data, 0, 1) ? true : false;
   const auto powerFailureState = bitset_extract(data, 1, 1) ? true : false;
   if (powerFailureSupported && !!powerFailure)
   {
      powerFailure->set(powerFailureState);
      historizers.push_back(powerFailure);
   }

   return historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_Common::extractActuatorStatusResponse2Channels(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   boost::shared_ptr<yApi::historization::CSwitch> channel1,
   boost::shared_ptr<yApi::historization::CSwitch> channel2,
   boost::shared_ptr<yApi::historization::CDimmable> dimmer,
   boost::shared_ptr<yApi::historization::CSwitch> powerFailure,
   boost::shared_ptr<yApi::historization::CSwitch> overCurrent)
{
   // Some devices supports several RORG telegrams, ignore non-VLD telegrams
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   if (bitset_extract(data, 4, 4) != kActuatorStatusResponse)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizers
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto ioChannel = bitset_extract(data, 11, 5);
   const int dimValue = bitset_extract(data, 17, 7);
   const auto state = dimValue == 0 ? false : true;
   const auto overCurrentState = bitset_extract(data, 8, 1) != 0;
   switch (ioChannel)
   {
   case 0:
      if (!!channel1)
      {
         channel1->set(state);
         historizers.push_back(channel1);
      }
      else if (!!dimmer)
      {
         dimmer->set(dimValue);
         historizers.push_back(dimmer);
      }
      else
      {
         YADOMS_LOG(warning) << "ActuatorStatusResponse : received unsupported ioChannel value " << ioChannel;
      }
      break;
   case 1:
      if (!!channel2)
      {
         channel2->set(state);
         historizers.push_back(channel2);
      }
      else
      {
         YADOMS_LOG(warning) << "ActuatorStatusResponse : received unsupported ioChannel value " << ioChannel;
      }
      break;
   default:
      YADOMS_LOG(warning) << "ActuatorStatusResponse : received unsupported ioChannel value " << ioChannel;
      break;
   }

   if (!!overCurrent)
   {
      overCurrent->set(overCurrentState);
      historizers.push_back(overCurrent);
   }

   const auto powerFailureSupported = bitset_extract(data, 0, 1) ? true : false;
   const auto powerFailureState = bitset_extract(data, 1, 1) ? true : false;
   if (powerFailureSupported && !!powerFailure)
   {
      powerFailure->set(powerFailureState);
      historizers.push_back(powerFailure);
   }

   return historizers;
}

void CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                              const std::string& senderId,
                                                              const std::string& targetId,
                                                              EOutputChannel outputChannel,
                                                              bool powerMeasurement,
                                                              bool resetMeasurement,
                                                              double minEnergyMeasureRefreshTime,
                                                              double maxEnergyMeasureRefreshTime)
{
   boost::dynamic_bitset<> data(6 * 8);

   bitset_insert(data, 4, 4, kActuatorSetMeasurement);
   bitset_insert(data, 8, true); // Report on query + auto reporting
   bitset_insert(data, 9, resetMeasurement); // Reset measurement
   bitset_insert(data, 10, powerMeasurement);
   bitset_insert(data, 11, 5, outputChannel); // Specific channel, or 0x1E for all output channels, or 0x1F for input channel
   bitset_insert(data, 16, 4, 0); // No measurement delta
   bitset_insert(data, 21, 3, powerMeasurement ? kPowerW : kEnergyWh); // Hard-coded for now
   bitset_insert(data, 24, 8, 0); // No measurement delta
   bitset_insert(data, 32, 8, static_cast<unsigned char>(maxEnergyMeasureRefreshTime / 10.0));
   bitset_insert(data, 40, 8, static_cast<unsigned char>(minEnergyMeasureRefreshTime));

   sendMessage(messageHandler,
               senderId,
               targetId,
               data,
               "Actuator Set Measurement");
}

void CProfile_D2_01_Common::sendActuatorMeasurementQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                                         const std::string& senderId,
                                                         const std::string& targetId,
                                                         EOutputChannel outputChannel,
                                                         EPowerQueryType queryType)
{
   boost::dynamic_bitset<> userData(2 * 8);
   bitset_insert(userData, 4, 4, kActuatorMeasurementQuery);
   bitset_insert(userData, 10, 1, queryType);
   bitset_insert(userData, 11, 5, outputChannel);

   sendMessage(messageHandler,
               senderId,
               targetId,
               userData,
               "Actuator Measurement Query");
}

const boost::shared_ptr<yApi::historization::CEnergy> CProfile_D2_01_Common::noInputEnergy = boost::shared_ptr<yApi::historization::CEnergy>();
const boost::shared_ptr<yApi::historization::CPower> CProfile_D2_01_Common::noInputPower = boost::shared_ptr<yApi::historization::CPower>();
const boost::shared_ptr<yApi::historization::CEnergy> CProfile_D2_01_Common::noLoadEnergy = boost::shared_ptr<yApi::historization::CEnergy>();
const boost::shared_ptr<yApi::historization::CPower> CProfile_D2_01_Common::noLoadPower = boost::shared_ptr<yApi::historization::CPower>();

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_Common::extractActuatorMeasurementResponse(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   boost::shared_ptr<yApi::historization::CEnergy> loadEnergy,
   boost::shared_ptr<yApi::historization::CPower> loadPower)
{
   // Some devices supports several RORG telegrams, ignore non-VLD telegrams
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto ioChannel = bitset_extract(data, 11, 5);
   const auto unit = static_cast<E_D2_01_MeasurementUnit>(bitset_extract(data, 8, 3));
   const auto rawValue = bitset_extract(data, 16, 32);

   switch (ioChannel)
   {
   case 0: // Output channel
      {
         switch (unit)
         {
         case kEnergyWs:
         case kEnergyWh:
         case kEnergyKWh:
            if (!!loadEnergy)
            {
               loadEnergy->set(extractEnergyWh(unit,
                                               rawValue));
               historizers.push_back(loadEnergy);
            }
            break;
         case kPowerW:
         case kPowerKW:
            if (!!loadPower)
            {
               loadPower->set(extractPowerValueW(unit,
                                                 rawValue));
               historizers.push_back(loadPower);
            }
            break;
         default:
            YADOMS_LOG(warning) << "ActuatorMeasurementResponse : received unsupported channel value " << ioChannel;
            break;
         }
         break;
      }
   default:
      YADOMS_LOG(warning) << "ActuatorMeasurementResponse : received unsupported channel value " << ioChannel;
      break;
   }
   return historizers;
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

void CProfile_D2_01_Common::sendActuatorPilotWireModeQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                                           const std::string& senderId,
                                                           const std::string& targetId)
{
   boost::dynamic_bitset<> userData(1 * 8);

   bitset_insert(userData, 4, 4, kActuatorPilotWireModeQuery);

   sendMessage(messageHandler,
               senderId,
               targetId,
               userData,
               "Actuator Pilot Wire Mode Query");
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_Common::extractActuatorPilotWireModeResponse(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   boost::shared_ptr<specificHistorizers::CPilotWireHistorizer> pilotWire)
{
   // Some devices supports several RORG telegrams, ignore non-VLD telegrams
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto pilotWireMode = static_cast<EPilotWireMode>(bitset_extract(data, 13, 3));
   switch (pilotWireMode)
   {
   case kOff:
      pilotWire->set(specificHistorizers::EPilotWire::kOff);
      historizers.push_back(pilotWire);
      break;
   case kComfort:
      pilotWire->set(specificHistorizers::EPilotWire::kComfort);
      historizers.push_back(pilotWire);
      break;
   case kEco:
      pilotWire->set(specificHistorizers::EPilotWire::kEco);
      historizers.push_back(pilotWire);
      break;
   case kAntiFreeze:
      pilotWire->set(specificHistorizers::EPilotWire::kAntiFreeze);
      historizers.push_back(pilotWire);
      break;
   case kComfort_1:
      pilotWire->set(specificHistorizers::EPilotWire::kComfort);
      historizers.push_back(pilotWire);
      break;
   case kComfort_2:
      pilotWire->set(specificHistorizers::EPilotWire::kComfort);
      historizers.push_back(pilotWire);
      break;
   default:
      YADOMS_LOG(warning) << "ActuatorPilotWireModeResponse : received unsupported pilotWireMode value " << pilotWireMode;
      break;
   }
   return historizers;
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
   boost::dynamic_bitset<> data(7 * 8);

   bitset_insert(data, 4, 4, kActuatorSetExternalInterfaceSettings);
   bitset_insert(data, 11, 5, outputChannel);
   bitset_insert(data, 16, 16, static_cast<unsigned int>(autoOffTimerSeconds * 10));
   bitset_insert(data, 32, 16, static_cast<unsigned int>(delayRadioOffTimerSeconds * 10));
   bitset_insert(data, 48, 2, connectedSwitchsType);
   bitset_insert(data, 50, !switchingStateToggle);

   sendMessage(messageHandler,
               senderId,
               targetId,
               data,
               "Actuator Set External Interface Settings");
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
      throw std::runtime_error((boost::format("Fail to send message to %1% : no answer to \"%2%\"") % targetId % commandName).str());

   const auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send message to " << targetId << " : \"" << commandName << "\" returns " << response->returnCode();
}


Poco::Int64 CProfile_D2_01_Common::extractEnergyWh(E_D2_01_MeasurementUnit unit,
                                                   unsigned int rawValue)
{
   switch (unit)
   {
   case kEnergyWs:
      return static_cast<Poco::Int64>(rawValue) * 3600;
   case kEnergyWh:
      return static_cast<Poco::Int64>(rawValue);
   case kEnergyKWh:
      return static_cast<Poco::Int64>(rawValue) * 1000;
   default:
      YADOMS_LOG(warning) << "extractEnergyWh : received unsupported unit value " << unit;
      return 0;
   }
}


double CProfile_D2_01_Common::extractPowerValueW(E_D2_01_MeasurementUnit unit,
                                                 unsigned int rawValue)
{
   switch (unit)
   {
   case kPowerW:
      return static_cast<double>(rawValue);
   case kPowerKW:
      return static_cast<double>(rawValue) * 1000;
   default:
      YADOMS_LOG(warning) << "extractPowerValueW : received unsupported unit value " << unit;
      return 0;
   }
}
