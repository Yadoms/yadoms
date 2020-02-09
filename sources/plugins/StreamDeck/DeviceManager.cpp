#include "DeviceManager.h"
#include "DeviceManagerHelper.h"

const uint16_t CDeviceManager::StreamDeckVendorId = 0x0FD9;

CDeviceManager::CDeviceManager(CConfiguration& configuration)
	: m_configuration(configuration)
{
}

std::list<std::shared_ptr<LibUSB::Device>> CDeviceManager::findDevice() const
{
	auto usbDeviceInformation = CDeviceManagerHelper::splitStringToVectorOfString(m_configuration.getUsbDevice(), ";");

	const auto vendorId = CDeviceManagerHelper::decimalToHex(usbDeviceInformation[0]);
	const auto productId = CDeviceManagerHelper::decimalToHex(usbDeviceInformation[1]);

	auto deviceList = LibUSB::LibUSB::FindDevice(vendorId, productId);

	if (deviceList.empty())
	{
		throw std::runtime_error("No compatible device found");
	}
	return deviceList;
}

std::list<std::shared_ptr<LibUSB::Device>> CDeviceManager::getStreamDeckDevices() const
{
	auto foundedDevice = LibUSB::LibUSB::FindAllDevices();
	std::list<std::shared_ptr<LibUSB::Device>> deviceList;

	for (const auto& device : foundedDevice)
	{
		if (StreamDeckVendorId == device->vendorID())
		{
			deviceList.push_back(device);
		}
	}
	if (deviceList.empty())
	{
		throw std::runtime_error("No compatible device found");
	}
	return deviceList;
}

boost::shared_ptr<UsbDeviceInformation> CDeviceManager::getDeviceInformation()
{
	auto deviceInformation = boost::make_shared<UsbDeviceInformation>();
	auto usbDevice = m_configuration.getUsbDevice();
	auto usbDeviceVid = CDeviceManagerHelper::findUsbDeviceId(usbDevice, "vid");
	auto usbDevicePid = CDeviceManagerHelper::findUsbDeviceId(usbDevice, "pid");
	
	if (CDeviceManagerHelper::getOsName() != "Windows")
	{
		auto usbDeviceInformation = CDeviceManagerHelper::splitStringToVectorOfString(usbDevice, ";");

		deviceInformation->vendorID = CDeviceManagerHelper::decimalToHex(usbDeviceInformation[0]);
		deviceInformation->productID = CDeviceManagerHelper::decimalToHex(usbDeviceInformation[1]);
		deviceInformation->serialNumber = usbDeviceInformation[2];
		deviceInformation->deviceModel = CDeviceManagerHelper::getDeviceModel(deviceInformation->vendorID, deviceInformation->productID);
		deviceInformation->keyCols = CDeviceManagerHelper::getDeviceKeyCols(deviceInformation->vendorID, deviceInformation->productID);
		deviceInformation->keyRows = CDeviceManagerHelper::getDeviceKeyRows(deviceInformation->vendorID, deviceInformation->productID);
		return deviceInformation;
	}

	deviceInformation->vendorID = CDeviceManagerHelper::stringToUnsignedShort(usbDeviceVid);
	deviceInformation->productID = CDeviceManagerHelper::stringToUnsignedShort(usbDevicePid);
	deviceInformation->serialNumber = CDeviceManagerHelper::getSerialNumber(usbDevice);
	deviceInformation->deviceModel = CDeviceManagerHelper::getDeviceModel(deviceInformation->vendorID, deviceInformation->productID);
	deviceInformation->keyCols = CDeviceManagerHelper::getDeviceKeyCols(deviceInformation->vendorID, deviceInformation->productID);
	deviceInformation->keyRows = CDeviceManagerHelper::getDeviceKeyRows(deviceInformation->vendorID, deviceInformation->productID);
	return deviceInformation;
}

