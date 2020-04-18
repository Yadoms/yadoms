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

	const auto res = hid_send_feature_report(m_handle, payload, 17);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when resetting device");
	}
}

void CStreamDeckMini::resetKeyStream()
{
	unsigned char payload[ImageReportLength] = {0x02};
	const auto res = hid_write(m_handle, payload, ImageReportLength);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to write an output report to a HID device when resetting key stream");
	}
}

void CStreamDeckMini::setBrightness(int percent)
{
	const auto percentInUnsignedCharValue = CDeviceManagerHelper::integerToHex(percent);
	
	unsigned char payload[17] = {0x05, 0x55, 0xaa, 0xd1, 0x01, percentInUnsignedCharValue};
	const auto res = hid_send_feature_report(m_handle, payload, 17);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when setting brightness");
	}
}
