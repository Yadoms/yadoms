#include "stdafx.h"
#include "ReadLearnModeCommand.h"

#include <ProtocolException.hpp>

#include "CommandSendMessage.h"
#include "shared/Log.h"

using namespace message::smart_ack;

CReadLearnModeCommand::CReadLearnModeCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
	: m_messageHandler(messageHandler)
{
}

void CReadLearnModeCommand::sendAndReceive()
{
	YADOMS_LOG(information) << "Read Smart Ack learn mode...";

	CCommandSendMessage sendMessage(CCommandSendMessage::SA_RD_LEARNMODE);

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

	static constexpr auto ExpectedDataSize = 3u;
	if (answer->header().dataLength() != ExpectedDataSize)
		throw CProtocolException(std::string("Invalid data length ")
								 + std::to_string(answer->header().dataLength())
								 + ", expected "
								 + std::to_string(ExpectedDataSize));

	processAnswer(response::CReceivedMessage(answer),
				  "SA_RD_LEARNMODE");
}

bool CReadLearnModeCommand::learnModeEnable() const
{
	return m_learnModeEnable;
}

message::LearnMode CReadLearnModeCommand::learnMode() const
{
	return m_learnMode;
}

void CReadLearnModeCommand::processAnswer(const response::CReceivedMessage& response,
										  const std::string& requestName)
{
	if (response.returnCode() != response::CReceivedMessage::RET_OK)
		throw CProtocolException("  ==> " + requestName + " request returned " + response::CReceivedMessage::toString(response.returnCode()));

	m_learnModeEnable = response.responseData()[1] != 0;
	m_learnMode = ToLearnMode(response.responseData()[1]);

	YADOMS_LOG(information) << "  ==> "
		<< (m_learnModeEnable ? "enable" : "disable")
		<< ", mode=" << ToString(m_learnMode);
}
