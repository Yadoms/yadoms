#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckOriginal : public CDeviceManager
{
public:
	explicit CStreamDeckOriginal(CConfiguration& configuration);

	virtual ~CStreamDeckOriginal();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

	void setKeyImage(std::string& content) override;

private:
	static const int ImageReportLength;

};

