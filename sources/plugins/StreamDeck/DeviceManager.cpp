#include "DeviceManager.h"
#include "DeviceManagerHelper.h"
#include <regex>

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
	if (CDeviceManagerHelper::getOsName() != "Windows")
	{
		auto usbDeviceInformation = CDeviceManagerHelper::splitStringToVectorOfString(usbDevice, ";");

		deviceInformation->vendorID = CDeviceManagerHelper::decimalToHex(usbDeviceInformation[0]);
		deviceInformation->productID = CDeviceManagerHelper::decimalToHex(usbDeviceInformation[1]);
		deviceInformation->serialNumber = usbDeviceInformation[2];
		return deviceInformation;
	}

	deviceInformation->vendorID = CDeviceManagerHelper::stringToUnsignedShort(findUsbDeviceId(usbDevice, "vid"));
	deviceInformation->productID = CDeviceManagerHelper::stringToUnsignedShort(findUsbDeviceId(usbDevice, "pid"));
	deviceInformation->serialNumber = getSerialNumber(usbDevice);
	return deviceInformation;
}


std::string CDeviceManager::findUsbDeviceId(std::string& value, const std::string& identifierToFind)
{
	std::smatch matches;
	const std::regex reg(identifierToFind + "_(\\w+)");
	if (!std::regex_search(value, matches, reg) || matches.empty())
	{
		throw;
	}
	return matches[1].str();
}

std::string CDeviceManager::getSerialNumber(std::string& value)
{
	const auto serialNumberLenght = 12;
	std::smatch matches;
	const std::regex reg("([[:alnum:]]+)");

	std::regex_token_iterator<std::string::iterator> rend;
	std::regex_token_iterator<std::string::iterator> a(value.begin(), value.end(), reg, 0);
	while (a != rend)
	{
		if (a->length() != serialNumberLenght)
		{
			a++;
			continue;
		}
		break;
	}
	return *a;
}
