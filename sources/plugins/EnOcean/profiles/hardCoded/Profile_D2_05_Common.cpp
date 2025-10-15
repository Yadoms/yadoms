#include "stdafx.h"
#include "Profile_D2_05_Common.h"
#include "../bitsetHelpers.hpp"
#include "message/radioErp1/SendMessage.h"
#include <shared/Log.h>
#include "message/MessageHelpers.h"


DECLARE_ENUM_IMPLEMENTATION_NESTED(CProfile_D2_05_Common::EAlarmAction, EAlarmAction,
                                   ((noAction))
                                   ((immediateStop))
                                   ((goUp))
                                   ((goDown))
                                   ((noChange))
);


void CProfile_D2_05_Common::sendGoToPositionAndAngle(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                                     const std::string& senderId,
                                                     const std::string& targetId,
                                                     boost::optional<yApi::historization::ECurtainCommand> curtain,
                                                     const specificHistorizers::EBlindLockingMode& lockingMode)
{
    boost::dynamic_bitset<> userData(4 * 8);

    unsigned int value = 127;
    if (curtain)
    {
        switch (curtain.get())
        {
        case yApi::historization::ECurtainCommand::kOpenValue:
            value = 0;
            break;
        case yApi::historization::ECurtainCommand::kCloseValue:
            value = 100;
            break;
        default:
            break;
        }
    }

    bitset_insert(userData, 1, 7, value);
    bitset_insert(userData, 9, 7, value);
    bitset_insert(userData, 17, 3, 0);
    bitset_insert(userData, 21, 3, lockingMode);
    bitset_insert(userData, 24, 4, 0);
    bitset_insert(userData, 28, 4, static_cast<int>(E_D2_01_Command::kGoToPositionAndAngle));

    sendMessage(messageHandler,
                senderId,
                targetId,
                userData,
                "Go to Position and Angle");
}

void CProfile_D2_05_Common::sendStop(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                     const std::string& senderId,
                                     const std::string& targetId)
{
    boost::dynamic_bitset<> userData(1 * 8);
    bitset_insert(userData, 0, 4, 0);
    bitset_insert(userData, 4, 4, static_cast<int>(E_D2_01_Command::kStop));

    sendMessage(messageHandler,
                senderId,
                targetId,
                userData,
                "Stop");
}

void CProfile_D2_05_Common::sendQueryPositionAndAngle(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                                      const std::string& senderId,
                                                      const std::string& targetId)
{
    boost::dynamic_bitset<> userData(1 * 8);
    bitset_insert(userData, 0, 4, 0);
    bitset_insert(userData, 4, 4, static_cast<int>(E_D2_01_Command::kQueryPositionAndAngle));

    sendMessage(messageHandler,
                senderId,
                targetId,
                userData,
                "Query Position and Angle");
}


std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_05_Common::
extractReplyPositionAndAngleResponse(unsigned char rorg,
                                     const boost::dynamic_bitset<>& data,
                                     boost::shared_ptr<yApi::historization::CCurtain> curtain,
                                     boost::shared_ptr<specificHistorizers::CBlindLockingMode> lockingMode)
{
    // Some devices supports several RORG telegrams, ignore non-VLD telegrams
    if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
        return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

    if (static_cast<E_D2_01_Command>(bitset_extract(data, 28, 4)) != E_D2_01_Command::kReplyPositionAndAngle)
        return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

    if (bitset_extract(data, 24, 4) != 0)
        return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

    // Return only the concerned historizer
    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

    const auto position = static_cast<int>(bitset_extract(data, 1, 7));
    const auto angle = static_cast<int>(bitset_extract(data, 9, 7));
    const auto locking = static_cast<specificHistorizers::EBlindLockingMode>(bitset_extract(data, 21, 3));

    if (position != 127)
    {
        curtain->set(position > 50
                         ? yApi::historization::ECurtainCommand::kOpen
                         : yApi::historization::ECurtainCommand::kClose);
        historizers.emplace_back(curtain);
    }
    else if (angle != 127)
    {
        curtain->set(angle > 50
                         ? yApi::historization::ECurtainCommand::kOpen
                         : yApi::historization::ECurtainCommand::kClose);
        historizers.emplace_back(curtain);
    }

    switch (locking)
    {
    case specificHistorizers::EBlindLockingMode::kDoNotChangeOrNoLockValue:
    case specificHistorizers::EBlindLockingMode::kBlockageValue:
    case specificHistorizers::EBlindLockingMode::kAlarmValue:
        lockingMode->set(locking);
        historizers.emplace_back(lockingMode);
        break;
    default:
        YADOMS_LOG(warning) << "ReplyPositionAndAngleResponse : received unsupported locking mode value " << locking;
        break;
    }

    return historizers;
}

void CProfile_D2_05_Common::sendSetParameters(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                              const std::string& senderId,
                                              const std::string& targetId,
                                              unsigned int measuredDurationOfVerticalRunMs,
                                              unsigned int measuredDurationOfRotationMs,
                                              const EAlarmAction& alarmAction)
{
    message::radioErp1::CSendMessage command(CRorgs::kVLD_Telegram,
                                             senderId,
                                             targetId,
                                             0);
    boost::dynamic_bitset<> data(5 * 8);

    if (measuredDurationOfVerticalRunMs < 5000 || measuredDurationOfVerticalRunMs > 300000)
        measuredDurationOfVerticalRunMs = 32767;
    else
        measuredDurationOfVerticalRunMs /= 10;

    if (measuredDurationOfRotationMs != 0)
    {
        if (measuredDurationOfRotationMs < 10 || measuredDurationOfRotationMs > 2540)
            measuredDurationOfRotationMs = 255;
        else
            measuredDurationOfRotationMs /= 10;
    }

    bitset_insert(data, 1, 15, measuredDurationOfVerticalRunMs);
    bitset_insert(data, 16, 8, measuredDurationOfRotationMs);
    bitset_insert(data, 29, 3, alarmAction);
    bitset_insert(data, 32, 4, 0);
    bitset_insert(data, 36, 4, static_cast<int>(E_D2_01_Command::kSetParameters));

    sendMessage(messageHandler,
                senderId,
                targetId,
                data,
                "Set parameters");
}


void CProfile_D2_05_Common::sendMessage(const boost::shared_ptr<IMessageHandler>& messageHandler,
                                        const std::string& senderId,
                                        const std::string& targetId,
                                        const boost::dynamic_bitset<>& userData,
                                        const std::string& commandName)
{
    message::CMessageHelpers::sendMessage(CRorgs::kVLD_Telegram,
                                          messageHandler,
                                          senderId,
                                          targetId,
                                          userData,
                                          commandName);
}
