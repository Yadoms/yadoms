#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../../IMessageHandler.h"
#include <specificHistorizers/BlindLockingMode.h>

namespace yApi = shared::plugin::yPluginApi;

class CProfile_D2_05_Common
{
public:
   enum class E_D2_01_Command
   {
      kGoToPositionAndAngle = 0x01,
      kStop = 0x02,
      kQueryPositionAndAngle = 0x03,
      kReplyPositionAndAngle = 0x04,
      kSetParameters = 0x05
   };

   DECLARE_ENUM_HEADER(EAlarmAction,
      ((noAction)(0))
      ((immediateStop)(1))
      ((goUp)(2))
      ((goDown)(3))
      ((noChange)(4))
   ) ;

   // CMD 0x1 - Actuator Set Output
   static void sendGoToPositionAndAngle(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                        const std::string& senderId,
                                        const std::string& targetId,
                                        boost::optional<yApi::historization::ECurtainCommand> curtain,
                                        const specificHistorizers::EBlindLockingMode& lockingMode);

   // CMD 0x2 - Stop
   static void sendStop(const boost::shared_ptr<IMessageHandler>& messageHandler,
                        const std::string& senderId,
                        const std::string& targetId);

   // CMD 0x03 - Query Position and Angle
   static void sendQueryPositionAndAngle(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                         const std::string& senderId,
                                         const std::string& targetId);

   // CMD 0x04 - Reply Position and Angle
   static const boost::shared_ptr<yApi::historization::CDimmable> NoVerticalPosition;
   static const boost::shared_ptr<yApi::historization::CDimmable> NoRotationAngle;
   static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> extractReplyPositionAndAngleResponse(unsigned char rorg,
                                                                                                                        const boost::dynamic_bitset<>& data,
                                                                                                                        boost::shared_ptr<yApi::historization::CCurtain> curtain,
                                                                                                                        boost::shared_ptr<specificHistorizers::CBlindLockingMode> lockingMode);

   // CMD 0x05 - Set parameters
   static void sendSetParameters(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                 const std::string& senderId,
                                 const std::string& targetId,
                                 unsigned int measuredDurationOfVerticalRunMs,
                                 unsigned int measuredDurationOfRotationMs,
                                 const EAlarmAction& alarmAction);

   static void sendMessage(const boost::shared_ptr<IMessageHandler>& messageHandler,
                           const std::string& senderId,
                           const std::string& targetId,
                           const boost::dynamic_bitset<>& userData,
                           const std::string& commandName);
};
