#include "StreamDeckMini.h"
#include "DeviceManagerHelper.h"

const int CStreamDeckMini::ImageReportLength = 1024;

CStreamDeckMini::CStreamDeckMini(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived)
	: CDeviceManager(configuration, mainEventHandler, evtKeyStateReceived)
{
}

CStreamDeckMini::~CStreamDeckMini() = default;

void CStreamDeckMini::reset()
{
	unsigned char payload[17];
	payload[0] = 0x0B;
	payload[1] = 0x63;

	hid_send_feature_report(m_handle, payload, 17);
}

void CStreamDeckMini::resetKeyStream()
{
	unsigned char payload[ImageReportLength] = {0x02};
	hid_write(m_handle, payload, ImageReportLength);
}

void CStreamDeckMini::setBrightness(int percent)
{
	unsigned char payload[17] = {0x05, 0x55, 0xaa, 0xd1, 0x01, CDeviceManagerHelper::integerToHex(percent)};
	hid_send_feature_report(m_handle, payload, 17);
}
