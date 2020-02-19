#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckOriginalV2 final : public CDeviceManager
{
public:
	explicit CStreamDeckOriginalV2(CConfiguration& configuration);

	virtual ~CStreamDeckOriginalV2();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

private:
	static const int ImageReportLength;
};
