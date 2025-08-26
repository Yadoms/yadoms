#include "stdafx.h"
#include "ReadLearnModeCommand.h"

#include <ProtocolException.hpp>

#include "CommonCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CReadLearnModeCommand::CReadLearnModeCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CReadLearnModeCommand::sendAndReceive()
    {
        YADOMS_LOG(information) << "Read learn mode...";

        CCommonCommandSendMessage sendMessage(CCommonCommandSendMessage::CO_RD_LEARNMODE);

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

        return processAnswer(response::CReceivedMessage(answer));
    }

    bool CReadLearnModeCommand::learnModeSupported() const
    {
        return m_learnModeSupported;
    }

    bool CReadLearnModeCommand::learnModeEnable() const
    {
        return m_learnModeEnable;
    }

    std::optional<unsigned int> CReadLearnModeCommand::channel() const
    {
        return m_channel;
    }

    void CReadLearnModeCommand::processAnswer(const response::CReceivedMessage& response)
    {
        if (response.returnCode() == response::CReceivedMessage::RET_NOT_SUPPORTED)
        {
            m_learnModeSupported = false;
            YADOMS_LOG(warning) << "  ==> " << response::CReceivedMessage::toString(response.returnCode());
            return;
        }

        m_learnModeSupported = true;

        if (response.returnCode() != response::CReceivedMessage::RET_OK)
        {
            YADOMS_LOG(warning) << "  ==> " << response::CReceivedMessage::toString(response.returnCode());
            throw CProtocolException("  ==> " + response::CReceivedMessage::toString(response.returnCode()));
        }

        m_learnModeEnable = response.responseData()[1] != 0;

        if (const auto channelValue = static_cast<unsigned int>(response.responseOptionalData()[0]); channelValue <= 0xFD)
            m_channel = channelValue;

        YADOMS_LOG(information) << "  ==> "
            << (m_learnModeEnable ? "enable" : "disable")
            << ", channel=" << (m_channel ? std::to_string(*m_channel) : ", not specified");
    }
} // namespace message
