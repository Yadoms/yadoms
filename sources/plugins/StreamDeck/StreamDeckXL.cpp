#include "StreamDeckXL.h"
#include "DeviceManagerHelper.h"

const int CStreamDeckXL::ImageReportLength = 1024;

CStreamDeckXL::CStreamDeckXL(CConfiguration& configuration)
	: CDeviceManager(configuration)
{
}

CStreamDeckXL::~CStreamDeckXL() = default;

void CStreamDeckXL::reset()
{
	unsigned char payload[32];
	payload[0] = 0x0B;
	payload[1] = 0x63;

	hid_send_feature_report(m_handle, payload, 32);
}

void CStreamDeckXL::resetKeyStream()
{
	unsigned char payload[ImageReportLength] = {0x02};
	hid_write(m_handle, payload, ImageReportLength);
}

void CStreamDeckXL::setBrightness(int percent)
{
	unsigned char payload[32] = {0x03, 0x08, CDeviceManagerHelper::integerToHex(percent)};
	hid_send_feature_report(m_handle, payload, 32);
}
