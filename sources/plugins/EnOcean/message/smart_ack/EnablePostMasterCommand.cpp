#include "stdafx.h"
#include "EnablePostMasterCommand.h"

#include <ProtocolException.hpp>

#include "CommandSendMessage.h"
#include "shared/Log.h"

using namespace message::smart_ack;

CEnablePostMasterCommand::CEnablePostMasterCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
	: m_messageHandler(messageHandler)
{
}

void CEnablePostMasterCommand::sendAndReceive(unsigned char mailboxesNumber) const
{
	YADOMS_LOG(information) << "Enable/disable Smart Ack mailboxes (" << static_cast<int>(mailboxesNumber) << " mailboxes)...";

	CCommandSendMessage sendMessage(CCommandSendMessage::SA_WR_POSTMASTER,
									{ mailboxesNumber });

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
		throw CProtocolException(std::string("Invalid data length ")
								 + std::to_string(answer->header().dataLength())
								 + ", expected "
								 + std::to_string(ExpectedDataSize));

	processAnswer(response::CReceivedMessage(answer).returnCode(),
				  "SA_WR_POSTMASTER");
}

void CEnablePostMasterCommand::processAnswer(const response::CReceivedMessage::EReturnCode returnCode,
											 const std::string& requestName)
{
	if (returnCode != response::CReceivedMessage::RET_OK)
		throw CProtocolException("  ==> " + requestName + " request returned " + response::CReceivedMessage::toString(returnCode));

	YADOMS_LOG(information) << "  ==> OK";
}
