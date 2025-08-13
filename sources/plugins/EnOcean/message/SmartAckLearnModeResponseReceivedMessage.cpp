#include "stdafx.h"
#include "SmartAckLearnModeResponseReceivedMessage.h"

#include "shared/Log.h"

namespace message
{
    CSmartAckLearnModeResponseReceivedMessage::CSmartAckLearnModeResponseReceivedMessage(const boost::shared_ptr<const CResponseReceivedMessage>& response)
        : m_learnModeEnable(response->responseData()[1] != 0),
          m_learnModeExtended(toLearnModeExtended(response->responseData()[1]))
    {
    }

    bool CSmartAckLearnModeResponseReceivedMessage::learnModeEnable() const
    {
        return m_learnModeEnable;
    }

    CSmartAckLearnModeResponseReceivedMessage::LearnModeExtended CSmartAckLearnModeResponseReceivedMessage::learnModeExtended() const
    {
        return m_learnModeExtended;
    }

    std::string CSmartAckLearnModeResponseReceivedMessage::toString(LearnModeExtended learnModeExtended)
    {
        switch (learnModeExtended)
        {
        case LearnModeExtended::SimpleLearnMode:
            return "Simple";
        case LearnModeExtended::AdvancedLearnMode:
            return "Advanced";
        case LearnModeExtended::AdvancedLearnModeSelectRepeater:
            return "Advanced, select repeater";
        default:
            return "Unknown";
        }
    }

    CSmartAckLearnModeResponseReceivedMessage::LearnModeExtended CSmartAckLearnModeResponseReceivedMessage::toLearnModeExtended(const unsigned char byte)
    {
        switch (byte)
        {
        case 0:
            return LearnModeExtended::SimpleLearnMode;
        case 1:
            return LearnModeExtended::AdvancedLearnMode;
        case 2:
            return LearnModeExtended::AdvancedLearnModeSelectRepeater;
        default:
            YADOMS_LOG(error) << boost::format("Smart Ack response : unsupported LearnModeExtended value %1%") % byte;
            return LearnModeExtended::SimpleLearnMode;
        }
    }
} // namespace message
