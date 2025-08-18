#include "stdafx.h"
#include "SmartAckPairingCommand.h"

#include <ProtocolException.hpp>

#include "SmartAckCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CSmartAckPairingCommand::CSmartAckPairingCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CSmartAckPairingCommand::sendAndReceive(const bool enable,
                                                 const LearnMode learnMode,
                                                 const unsigned int timeoutMs) const
    {
        YADOMS_LOG(information) << "Enable/disable Smart Ack pairing ("
            << (enable ? "enable" : "disable")
            << ", " << ToString(learnMode)
            << ", " << timeoutMs << "ms)... ";

        CSmartAckCommandSendMessage sendMessage(
            CSmartAckCommandSendMessage::SA_WR_LEARNMODE,
            {
                static_cast<unsigned char>(enable ? 1 : 0),
                ToProtocolValue(learnMode),
                static_cast<unsigned char>(timeoutMs >> 24 & 0xff),
                static_cast<unsigned char>(timeoutMs >> 16 & 0xff),
                static_cast<unsigned char>(timeoutMs >> 8 & 0xff),
                static_cast<unsigned char>(timeoutMs & 0xff)
            });

        boost::shared_ptr<const CEsp3ReceivedPacket> answer;
        if (!m_messageHandler->send(sendMessage,
                                    [](const boost::shared_ptr<const CEsp3ReceivedPacket>& esp3Packet)
                                    {
                                        if (esp3Packet->header().packetType() == RESPONSE)
                                            return true;
                                        YADOMS_LOG(warning) << "Unexpected message received : wrong packet type : " << esp3Packet->header().
                                            packetType();
                                        return false;
                                    },
                                    [&](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                                    {
                                        answer = std::move(esp3Packet);
                                    }))
            throw CProtocolException("Timeout waiting answer");

        processAnswer(CResponseReceivedMessage(answer).returnCode(),
                      "SA_WR_LEARNMODE");
    }

    void CSmartAckPairingCommand::processAnswer(const CResponseReceivedMessage::EReturnCode returnCode,
                                                const std::string& requestName)
    {
        if (returnCode != CResponseReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + CResponseReceivedMessage::toString(returnCode));

        YADOMS_LOG(information) << "  ==> OK";
    }
} // namespace message
