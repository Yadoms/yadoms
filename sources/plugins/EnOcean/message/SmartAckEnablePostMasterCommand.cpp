#include "stdafx.h"
#include "SmartAckEnablePostMasterCommand.h"

#include <ProtocolException.hpp>

#include "SmartAckCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CSmartAckEnablePostMasterCommand::CSmartAckEnablePostMasterCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CSmartAckEnablePostMasterCommand::sendAndReceive(unsigned char mailboxesNumber) const
    {
        YADOMS_LOG(information) << "Enable/disable Smart Ack mailboxes (" << mailboxesNumber << " mailboxes)...";

        CSmartAckCommandSendMessage sendMessage(CSmartAckCommandSendMessage::SA_WR_POSTMASTER,
                                                {mailboxesNumber});

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

        static constexpr auto ResponseSmartAckEnablePostmasterSize = 1u;
        if (answer->header().dataLength() != ResponseSmartAckEnablePostmasterSize)
            throw CProtocolException(
                (boost::format("Invalid data length %1%, expected %2%")
                    % answer->header().dataLength()
                    % ResponseSmartAckEnablePostmasterSize).str());

        processAnswer(CResponseReceivedMessage(answer).returnCode(),
                      "SA_WR_POSTMASTER");
    }

    void CSmartAckEnablePostMasterCommand::processAnswer(const CResponseReceivedMessage::EReturnCode returnCode,
                                                         const std::string& requestName)
    {
        if (returnCode != CResponseReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + CResponseReceivedMessage::toString(returnCode));

        YADOMS_LOG(information) << "  ==> OK";
    }
} // namespace message
