#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckXL final : public CDeviceManager
{
public:
	explicit CStreamDeckXL(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived);

	virtual ~CStreamDeckXL();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

	void setKeyImage(std::string& content, int& keyIndex, std::string& customText) override;

	std::pair<bool, int> readKeyStates() override;

private:
	static const int ImageReportLength;
	static const int KeyCols;
	static const int KeyRows;
	static const int KeyCount;
	static const int DataToSendLength;
	static const int KeyPixelSize;
	static const int ImageReportHeaderLength;
	static const int ImageReportPayloadLength;
};

