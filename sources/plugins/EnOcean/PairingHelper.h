#pragma once
#include "IPairingHelper.h"
#include "IMessageHandler.h"

//--------------------------------------------------------------
/// \brief	The default pairing helper implementation (based on manual/auto mode)
//--------------------------------------------------------------
class CPairingHelper final : public IPairingHelper
{
public:
	explicit CPairingHelper(const boost::shared_ptr<IMessageHandler>& messageHandler);
	~CPairingHelper() override = default;

	// IPairingHelper implementation
	bool isPairing() override;
	bool start() override;
	void stop() override;
	// [END] IPairingHelper implementation

protected:
	void startLearnMode();
	void stopLearnMode() const;

	void startSmartAckPairing() const;
	void stopSmartAckPairing() const;

private:
	bool m_pairingEnable = false;
	boost::shared_ptr<IMessageHandler> m_messageHandler;
	bool m_learnByDongleSupported = false;
};
