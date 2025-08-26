#include "stdafx.h"
#include "SmartAckReadClientMailboxStatusCommand.h"

#include <ProtocolException.hpp>

#include "SmartAckCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CSmartAckReadClientMailboxStatusCommand::CSmartAckReadClientMailboxStatusCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CSmartAckReadClientMailboxStatusCommand::sendAndReceive(const std::string& clientId,
                                                                 const std::string& controllerId)
    {
        YADOMS_LOG(information) << "Read Smart Ack client mailbox status (controller " << controllerId << ", client " << clientId << ")...";

        CSmartAckCommandSendMessage sendMessage(
            CSmartAckCommandSendMessage::SA_RD_MAILBOX,
            {
                static_cast<unsigned char>(std::stoul(clientId.substr(0, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(clientId.substr(2, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(clientId.substr(4, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(clientId.substr(6, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(controllerId.substr(0, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(controllerId.substr(2, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(controllerId.substr(4, 2), nullptr, 16)),
                static_cast<unsigned char>(std::stoul(controllerId.substr(6, 2), nullptr, 16))
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

        static constexpr auto ExpectedResponseSize = 1u;
        if (answer->header().dataLength() != ExpectedResponseSize)
            throw CProtocolException(
                (boost::format("Invalid data length %1%, expected %2%")
                    % answer->header().dataLength()
                    % ExpectedResponseSize).str());

        processAnswer(response::CReceivedMessage(answer),
                      "SA_RD_MAILBOX");
    }

    CSmartAckReadClientMailboxStatusCommand::MailboxStatus CSmartAckReadClientMailboxStatusCommand::status() const
    {
        return m_status;
    }

    std::string CSmartAckReadClientMailboxStatusCommand::toString(MailboxStatus mailboxStatus)
    {
        switch (mailboxStatus)
        {
        case EMPTY:
            return "Empty";
        case FULL:
            return "Full";
        case DOESNT_EXIST:
            return "Doesn't exist";
        default:
            return "";
        }
    }

    void CSmartAckReadClientMailboxStatusCommand::processAnswer(const response::CReceivedMessage& response,
                                                                const std::string& requestName)
    {
        if (response.returnCode() != response::CReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + response::CReceivedMessage::toString(response.returnCode()));

        m_status = toMailboxStatus(response.responseData()[0]);

        YADOMS_LOG(information) << "  ==> " << toString(m_status);
    }

    CSmartAckReadClientMailboxStatusCommand::MailboxStatus CSmartAckReadClientMailboxStatusCommand::toMailboxStatus(unsigned char byte)
    {
        switch (byte)
        {
        case 0:
            return EMPTY;
        case 1:
            return FULL;
        case 2:
            return DOESNT_EXIST;
        default:
            YADOMS_LOG(error) << boost::format("Smart Ack response : unsupported mailbox status value %1%") % byte;
            return DOESNT_EXIST;
        }
    }
} // namespace message
