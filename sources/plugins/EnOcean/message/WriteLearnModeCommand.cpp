#include "stdafx.h"
#include "WriteLearnModeCommand.h"

#include <ProtocolException.hpp>

#include "CommonCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CWriteLearnModeCommand::CWriteLearnModeCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CWriteLearnModeCommand::sendAndReceive(const bool enable,
                                                const unsigned int timeoutMs) const
    {
        YADOMS_LOG(information) << "Write learn mode ("
            << (enable ? "enable" : "disable")
            << ", " << timeoutMs << "ms)... ";

        CCommonCommandSendMessage sendMessage(CCommonCommandSendMessage::CO_WR_LEARNMODE,
                                              {
                                                  static_cast<unsigned char>(enable ? 1 : 0),
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

        static constexpr auto ExpectedDataSize = 1u;
        if (answer->header().dataLength() != ExpectedDataSize)
            throw CProtocolException(
                (boost::format("Invalid data length %1%, expected %2%")
                    % answer->header().dataLength()
                    % ExpectedDataSize).str());

        processAnswer(response::CReceivedMessage(answer),
                      "CO_WR_LEARNMODE");
    }

    void CWriteLearnModeCommand::processAnswer(const response::CReceivedMessage& response,
                                               const std::string& requestName)
    {
        if (response.returnCode() != response::CReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + response::CReceivedMessage::toString(response.returnCode()));
    }
} // namespace message
