#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include "../../IMessageHandler.h"
#include "../generated-eep.h"


class CProfile_D2_01_Common
{
public:
   DECLARE_ENUM_HEADER(EDefaultState,
      ((off)(0))
      ((on)(1))
      ((previousState)(2))
      ((notUsed)(3))
   );

   DECLARE_ENUM_HEADER(EConnectedSwitchsType,
      ((switch)(1))
      ((pushButton)(2))
      ((autodetection)(3))
   );

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

   // CMD 0x2 - Actuator Set Local
   static void sendActuatorSetLocalCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                           const std::string& senderId,
                                           const std::string& targetId,
                                           const CRorgs::ERorgIds& rorg,
                                           bool localControl,
                                           bool taughtInAllDevices,
                                           bool userInterfaceDayMode,
                                           const EDefaultState& defaultState);

   // CMD 0xB - Actuator Set External Interface Settings
   static void sendActuatorSetExternalInterfaceSettingsCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                               const std::string& senderId,
                                                               const std::string& targetId,
                                                               const CRorgs::ERorgIds& rorg,
                                                               const EConnectedSwitchsType& connectedSwitchsType,
                                                               double autoOffTimerSeconds,
                                                               double delayRadioOffTimerSeconds,
                                                               bool switchingStateToggle);

   // CMD 0x5 - Actuator Set Measurement
   static void sendActuatorSetMeasurementCommand(boost::shared_ptr<IMessageHandler> messageHandler,
                                                 const std::string& senderId,
                                                 const std::string& targetId,
                                                 const CRorgs::ERorgIds& rorg,
                                                 bool powerMeasurement,
                                                 bool outputChannels,
                                                 double minRefreshTime,
                                                 double maxRefreshTime);
};
