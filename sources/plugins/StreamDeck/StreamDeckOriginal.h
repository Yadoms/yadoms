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

	void setKeyImage(std::string& content, int& keyIndex, std::string& customText) override;

	void readKeyStates() override;

private:
	static const int ImageReportLength;
	static const int KeyCols;
	static const int KeyRows;
	static const int KeyCount;
	static const int DataToSendLength;
	static int convertKeyIdOrigin(int& keyIndex);
};

