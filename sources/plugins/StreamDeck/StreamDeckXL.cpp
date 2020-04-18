#include "StreamDeckXL.h"
#include "DeviceManagerHelper.h"

const int CStreamDeckXL::ImageReportLength = 1024;

CStreamDeckXL::CStreamDeckXL(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler,
                             int evtKeyStateReceived)
	: CDeviceManager(configuration, mainEventHandler, evtKeyStateReceived)
{
}

CStreamDeckXL::~CStreamDeckXL() = default;

void CStreamDeckXL::reset()
{
	unsigned char payload[32];
	payload[0] = 0x0B;
	payload[1] = 0x63;

	const auto res = hid_send_feature_report(m_handle, payload, 32);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when resetting device");
	}
}

void CStreamDeckXL::resetKeyStream()
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

void CStreamDeckXL::setBrightness(int percent)
{
	const auto percentInUnsignedCharValue = CDeviceManagerHelper::integerToHex(percent);
	unsigned char payload[32] = {0x03, 0x08, percentInUnsignedCharValue};
	const auto res = hid_send_feature_report(m_handle, payload, 32);

	if (res == CDeviceManagerHelper::EHidError::kTrue)
	{
		hid_close(m_handle);
		hid_exit();
		throw std::runtime_error("HID ERROR : Unable to send hid feature report when setting brightness");
	}
}
