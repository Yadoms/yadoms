#include "DeviceManager.h"
#include "DeviceManagerHelper.h"
#include <hidapi.h>

const uint16_t CDeviceManager::StreamDeckVendorId = 0x0FD9;

CDeviceManager::CDeviceManager(CConfiguration& configuration)
	: m_configuration(configuration), m_handle(nullptr)
{
}

void CDeviceManager::open()
{
	m_handle = hid_open(CDeviceManagerHelper::getDeviceInformation(m_configuration)->vendorID, CDeviceManagerHelper::getDeviceInformation(m_configuration)->productID, nullptr);
}

