#include "stdafx.h"
#include "PairingCommand.h"

#include <ProtocolException.hpp>

#include "CommandSendMessage.h"
#include "shared/Log.h"

using namespace message::smart_ack;

CPairingCommand::CPairingCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
    : m_messageHandler(messageHandler)
{
}

void CPairingCommand::sendAndReceive(const bool enable,
                                     const LearnMode learnMode,
                                     const unsigned int timeoutMs) const
{
    YADOMS_LOG(information) << "Enable/disable Smart Ack pairing ("
        << (enable ? "enable" : "disable")
        << ", " << ToString(learnMode)
        << ", " << timeoutMs << "ms)... ";

    CCommandSendMessage sendMessage(
        CCommandSendMessage::SA_WR_LEARNMODE,
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

    processAnswer(response::CReceivedMessage(answer).returnCode(),
                  "SA_WR_LEARNMODE");
}

void CPairingCommand::processAnswer(const response::CReceivedMessage::EReturnCode returnCode,
                                    const std::string& requestName)
{
    if (returnCode != response::CReceivedMessage::RET_OK)
        throw CProtocolException("  ==> " + requestName + " request returned " + response::CReceivedMessage::toString(returnCode));

    YADOMS_LOG(information) << "  ==> OK";
}
