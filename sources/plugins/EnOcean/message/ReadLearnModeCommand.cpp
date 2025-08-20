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

        static constexpr auto ExpectedDataSize = 2u;
        if (answer->header().dataLength() != ExpectedDataSize)
            throw CProtocolException(
                (boost::format("Invalid data length %1%, expected %2%")
                    % answer->header().dataLength()
                    % ExpectedDataSize).str());

        static constexpr auto ExpectedOptionalDataSize = 1u;
        if (answer->header().dataLength() != ExpectedOptionalDataSize)
            throw CProtocolException(
                (boost::format("Invalid optional data length %1%, expected %2%")
                    % answer->header().dataLength()
                    % ExpectedOptionalDataSize).str());

        processAnswer(CResponseReceivedMessage(answer),
                      "CO_RD_LEARNMODE");
    }

    bool CReadLearnModeCommand::learnModeEnable() const
    {
        return m_learnModeEnable;
    }

    std::optional<unsigned int> CReadLearnModeCommand::channel() const
    {
        return m_channel;
    }

    void CReadLearnModeCommand::processAnswer(const CResponseReceivedMessage& response,
                                              const std::string& requestName)
    {
        if (response.returnCode() != CResponseReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + CResponseReceivedMessage::toString(response.returnCode()));

        m_learnModeEnable = response.responseData()[1] != 0;

        if (response.responseOptionalData()[0] >= 0 && response.responseOptionalData()[0] <= 0xFD)
            m_channel = response.responseOptionalData()[0];

        YADOMS_LOG(information) << "  ==> "
            << (m_learnModeEnable ? "enable" : "disable")
            << ", channel=" << (m_channel ? std::to_string(*m_channel) : ", not specified");
    }
} // namespace message
