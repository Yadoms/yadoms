#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "../../IMessageHandler.h"
#include <specificHistorizers/PilotWireHistorizer.h>
#include <specificHistorizers/DimmerModeHistorizer.h>

namespace yApi = shared::plugin::yPluginApi;

class CProfile_D2_01_Common
{
public:
   DECLARE_ENUM_HEADER(EDefaultState,
      ((off)(0))
      ((on)(1))
      ((previousState)(2))
      ((notUsed)(3))
   ) ;

   DECLARE_ENUM_HEADER(EConnectedSwitchsType,
      ((externalSwitch)(1))
      ((externalPushButton)(2))
      ((autodetection)(3))
   ) ;

   enum E_D2_01_Command
   {
      kActuatorSetOutput = 0x01,
      kActuatorSetLocal = 0x02,
      kActuatorStatusQuery = 0x03,
      kActuatorStatusResponse = 0x04,
      kActuatorSetMeasurement = 0x05,
      kActuatorMeasurementQuery = 0x06,
      kActuatorMeasurementResponse = 0x07,
      kActuatorSetPilotWireMode = 0x08,
      kActuatorPilotWireModeQuery = 0x09,
      kActuatorPilotWireModeResponse = 0x0A,
      kActuatorSetExternalInterfaceSettings = 0x0B,
      kActuatorExternalInterfaceSettingsQuery = 0x0C,
      kActuatorExternalInterfaceSettingsResponse = 0x0D
   };

   enum E_D2_01_MeasurementUnit
   {
      kEnergyWs = 0x00,
      kEnergyWh = 0x01,
      kEnergyKWh= 0x02,
      kPowerW = 0x03,
      kPowerKW = 0x04
   };

   enum E_D2_01_DimMode
   {
      kSwitchToValue = 0x00,
      kDimToValueWithTimer1 = 0x01,
      kDimToValueWithTimer2 = 0x02,
      kDimToValueWithTimer3 = 0x03,
      kStopDimming = 0x04,
   };

   enum EPilotWireMode
   {
      kOff = 0x00,
      kComfort = 0x01,
      kEco = 0x02,
      kAntiFreeze = 0x03,
      kComfort_1 = 0x04,
      kComfort_2 = 0x05
   };

   enum EOutputChannel
   {
      kOutputChannel1 = 0,
      kOutputChannel2 = 1,
      kAllOutputChannels = 0x1E,
      kInputChannel = 0x1F,
   };

   enum EPowerQueryType
   {
      kQueryEnergy = 0,
      kQueryPower = 1
   };

   // CMD 0x1 - Actuator Set Output
   static void sendActuatorSetOutputCommandSwitching(boost::shared_ptr<IMessageHandler> messageHandler,
                                                     const std::string& senderId,
                                                     const std::string& targetId,
                                                     EOutputChannel outputChannel,
                                                     bool state);
   static void sendActuatorSetOutputCommandDimming(boost::shared_ptr<IMessageHandler> messageHandler,
                                                   const std::string& senderId,
                                                   const std::string& targetId,
                                                   EOutputChannel outputChannel,
                                                   const specificHistorizers::EDimmerMode& mode,
                                                   unsigned int dimValue);

   // CMD 0x2 - Actuator Set Local
   static void sendActuatorSetLocalCommand(boost::shared_ptr<IMessageHandler> messageHandler,
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
                                           double dimTimer3);

   // CMD 0x04 - Actuator Status Query
   static void sendActuatorStatusQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                       const std::string& senderId,
                                       const std::string& targetId,
                                       EOutputChannel outputChannel);

   // CMD 0x04 - Actuator Status Response
   static const boost::shared_ptr<yApi::historization::CSwitch> noChannel1;
   static const boost::shared_ptr<yApi::historization::CSwitch> noChannel2;
   static const boost::shared_ptr<yApi::historization::CDimmable> noDimmable;
   static const boost::shared_ptr<yApi::historization::CSwitch> noPowerFailure;
   static const boost::shared_ptr<yApi::historization::CSwitch> noOverCurrent;
   static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> extractActuatorStatusResponse(
      unsigned char rorg,
      const boost::dynamic_bitset<>& data,
      boost::shared_ptr<yApi::historization::CSwitch> channel1,
      boost::shared_ptr<yApi::historization::CDimmable> dimmer,
      boost::shared_ptr<yApi::historization::CSwitch> powerFailure,
      boost::shared_ptr<yApi::historization::CSwitch> overCurrent);
   static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> extractActuatorStatusResponse2Channels(
      unsigned char rorg,
      const boost::dynamic_bitset<>& data,
      boost::shared_ptr<yApi::historization::CSwitch> channel1,
      boost::shared_ptr<yApi::historization::CSwitch> channel2,
      boost::shared_ptr<yApi::historization::CDimmable> dimmer,
      boost::shared_ptr<yApi::historization::CSwitch> powerFailure,
      boost::shared_ptr<yApi::historization::CSwitch> overCurrent);

   // CMD 0x5 - Actuator Set Measurement
   static void sendActuatorSetMeasurementCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                 const std::string& senderId,
                                                 const std::string& targetId,
                                                 EOutputChannel outputChannel,
                                                 bool powerMeasurement,
                                                 bool resetMeasurement,
                                                 double minEnergyMeasureRefreshTime,
                                                 double maxEnergyMeasureRefreshTime);

   // CMD 0x6 - Actuator Measurement Query
   static void sendActuatorMeasurementQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                            const std::string& senderId,
                                            const std::string& targetId,
                                            EOutputChannel outputChannel,
                                            EPowerQueryType queryType);

   // CMD 0x07 - Actuator Measurement Response
   static const boost::shared_ptr<yApi::historization::CEnergy> noInputEnergy;
   static const boost::shared_ptr<yApi::historization::CPower> noInputPower;
   static const boost::shared_ptr<yApi::historization::CEnergy> noLoadEnergy;
   static const boost::shared_ptr<yApi::historization::CPower> noLoadPower;
   static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> extractActuatorMeasurementResponse(
      unsigned char rorg,
      const boost::dynamic_bitset<>& data,
      boost::shared_ptr<yApi::historization::CEnergy> loadEnergy,
      boost::shared_ptr<yApi::historization::CPower> loadPower);

   // CMD 0x8 - Actuator Set Pilot Wire Mode
   static void sendActuatorSetPilotWireModeCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                   const std::string& senderId,
                                                   const std::string& targetId,
                                                   const specificHistorizers::EPilotWire& mode);

   // CMD 0x9 - Actuator Pilot Wire Mode Query
   static void sendActuatorPilotWireModeQuery(boost::shared_ptr<IMessageHandler> messageHandler,
                                              const std::string& senderId,
                                              const std::string& targetId);

   // CMD 0xA - Actuator Pilot Wire Mode Response
   static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> extractActuatorPilotWireModeResponse(
      unsigned char rorg,
      const boost::dynamic_bitset<>& data,
      boost::shared_ptr<specificHistorizers::CPilotWireHistorizer> pilotWire);

   // CMD 0xB - Actuator Set External Interface Settings
   static void sendActuatorSetExternalInterfaceSettingsCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                               const std::string& senderId,
                                                               const std::string& targetId,
                                                               EOutputChannel outputChannel,
                                                               const EConnectedSwitchsType& connectedSwitchsType,
                                                               double autoOffTimerSeconds,
                                                               double delayRadioOffTimerSeconds,
                                                               bool switchingStateToggle);

   static void sendMessage(boost::shared_ptr<IMessageHandler> messageHandler,
                           const std::string& senderId,
                           const std::string& targetId,
                           const boost::dynamic_bitset<>& userData,
                           const std::string& commandName);

   static Poco::Int64 extractEnergyWh(E_D2_01_MeasurementUnit unit,
                                      unsigned int rawValue);

   static double extractPowerValueW(E_D2_01_MeasurementUnit unit,
                                    unsigned int rawValue);
};
