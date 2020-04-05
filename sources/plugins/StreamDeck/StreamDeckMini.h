#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckMini final : public CDeviceManager
{
public:
	explicit CStreamDeckMini(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived);

	virtual ~CStreamDeckMini();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

private:
	static const int ImageReportLength;
};
