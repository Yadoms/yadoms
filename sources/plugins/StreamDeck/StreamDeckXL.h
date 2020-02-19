#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckXL final : public CDeviceManager
{
public:
	explicit CStreamDeckXL(CConfiguration& configuration);

	virtual ~CStreamDeckXL();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

private:
	static const int ImageReportLength;
};

