#include "stdafx.h"
#include "PairingHelper.h"

#include "message/ReadLearnModeCommand.h"
#include "message/smart_ack/PairingCommand.h"
#include "message/WriteLearnModeCommand.h"


CPairingHelper::CPairingHelper(const boost::shared_ptr<IMessageHandler>& messageHandler)
	: m_messageHandler(messageHandler)
{
}

bool CPairingHelper::isPairing()
{
	return m_pairingEnable;
}

bool CPairingHelper::start()
{
	m_pairingEnable = true;

	YADOMS_LOG(information) << "Start pairing...";

	startLearnMode();
	startSmartAckPairing();

	YADOMS_LOG(information) << "Pairing started";

	return true;
}

void CPairingHelper::stop()
{
	m_pairingEnable = false;

	YADOMS_LOG(information) << "Stop pairing...";

	stopSmartAckPairing();
	stopLearnMode();

	YADOMS_LOG(information) << "Pairing stopped";
}

void CPairingHelper::startLearnMode()
{
	message::CReadLearnModeCommand readLearnModeCmd(m_messageHandler);
	readLearnModeCmd.sendAndReceive();
	m_learnByDongleSupported = readLearnModeCmd.learnModeSupported();

	YADOMS_LOG(information) << "Learn mode " << (m_learnByDongleSupported ? "" : "not ") << "supported";

	if (!m_learnByDongleSupported || !readLearnModeCmd.learnModeEnable())
		return;

	const message::CWriteLearnModeCommand writeLearnModeCmd(m_messageHandler);
	writeLearnModeCmd.sendAndReceive(true);
}

void CPairingHelper::stopLearnMode() const
{
	if (!m_learnByDongleSupported)
		return;

	const message::CWriteLearnModeCommand cmd(m_messageHandler);
	cmd.sendAndReceive(false);
}

void CPairingHelper::startSmartAckPairing() const
{
	const message::smart_ack::CPairingCommand cmd(m_messageHandler);
	cmd.sendAndReceive(true,
					   message::LearnMode::SimpleLearnMode);
}

void CPairingHelper::stopSmartAckPairing() const
{
	const message::smart_ack::CPairingCommand cmd(m_messageHandler);
	cmd.sendAndReceive(false,
					   message::LearnMode::SimpleLearnMode);
}
