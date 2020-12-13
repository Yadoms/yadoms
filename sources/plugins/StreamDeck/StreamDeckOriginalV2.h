#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckOriginalV2 final : public CDeviceManager
{
public:
	explicit CStreamDeckOriginalV2(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived);

	virtual ~CStreamDeckOriginalV2();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

	void setKeyImage(std::string content, int& keyIndex, std::string customText) override;

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
