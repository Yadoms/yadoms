#include "StreamDeckOriginalV2.h"
#include "DeviceManagerHelper.h"

const int CStreamDeckOriginalV2::ImageReportLength = 1024;

CStreamDeckOriginalV2::CStreamDeckOriginalV2(CConfiguration& configuration)
	: CDeviceManager(configuration)
{
}

CStreamDeckOriginalV2::~CStreamDeckOriginalV2() = default;

void CStreamDeckOriginalV2::reset()
{
	unsigned char payload[32];
	payload[0] = 0x03;
	payload[1] = 0x02;

	hid_send_feature_report(m_handle, payload, 32);
}

void CStreamDeckOriginalV2::resetKeyStream()
{
	unsigned char payload[ImageReportLength] = {0x02};
	hid_write(m_handle, payload, ImageReportLength);
}

void CStreamDeckOriginalV2::setBrightness(int percent)
{
	unsigned char payload[32] = {0x03, 0x08, CDeviceManagerHelper::integerToHex(percent)};
	hid_send_feature_report(m_handle, payload, 32);
}
