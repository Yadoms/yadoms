#include "DeviceManager.h"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <regex>

CDeviceManager::CDeviceManager() = default;
const uint16_t CDeviceManager::StreamDeckVendorId = 0x0FD9;

std::list<std::shared_ptr<LibUSB::Device>> CDeviceManager::findDevice(CConfiguration& configuration)
{
	auto usbDeviceInformation = splitStringToVectorOfString(configuration.getUsbDevice(), ";");

	const auto vendorId = decimalToHex(usbDeviceInformation[0]);
	const auto productId = decimalToHex(usbDeviceInformation[1]);

	auto deviceList = LibUSB::LibUSB::FindDevice(vendorId, productId);

	if (deviceList.empty())
	{
		throw std::runtime_error("No compatible device found");
	}
	return deviceList;
}

std::list<std::shared_ptr<LibUSB::Device>> CDeviceManager::getStreamDeckDevices()
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

// TODO : Move this Functions to a Helper
std::vector<std::string> CDeviceManager::splitStringToVectorOfString(const std::string& wordToSplit,
                                                                  const std::string& separator)
{
	std::vector<std::string> words;
	return split(words, wordToSplit, boost::is_any_of(separator), boost::token_compress_on);
}

uint16_t CDeviceManager::decimalToHex(std::string& decimalValue)
{
	boost::cnv::cstream converter;
	return boost::convert<uint16_t>(decimalValue, converter(std::showbase)(std::uppercase)(std::dec), 0);
}

boost::shared_ptr<UsbDeviceInformation> CDeviceManager::getDeviceInformation(CConfiguration& configuration)
{
	auto deviceInformation = boost::make_shared<UsbDeviceInformation>();
	auto usbDevice = configuration.getUsbDevice();
	if (getOsName() != "Windows")
	{
		auto usbDeviceInformation = splitStringToVectorOfString(usbDevice, ";");

		deviceInformation->vendorID = decimalToHex(usbDeviceInformation[0]);
		deviceInformation->productID = decimalToHex(usbDeviceInformation[1]);
		deviceInformation->serialNumber = usbDeviceInformation[2];
		return deviceInformation;
	}

	deviceInformation->vendorID = stringToUnsignedShort(findUsbDeviceId(usbDevice, "vid"));
	deviceInformation->productID = stringToUnsignedShort(findUsbDeviceId(usbDevice, "pid"));
	deviceInformation->serialNumber = getSerialNumber(usbDevice);
	return deviceInformation;
}

std::string CDeviceManager::getOsName()
{
#ifdef WIN32
	return "Windows";
#elif __APPLE__ || __MACH__
	return "Mac OSX";
#elif __linux__
	return "Linux";
#elif __FreeBSD__
	return "FreeBSD";
#elif __unix || __unix__
	return "Unix";
#else
	return "Other";
#endif
}

uint16_t CDeviceManager::stringToUnsignedShort(std::string& value)
{
	return static_cast<uint16_t>(std::stoi(value, nullptr, 16));
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
	std::regex_token_iterator<std::string::iterator> a(value.begin(), value.end(), reg,0);
	while (a != rend) {
		if (a->length() != serialNumberLenght)
		{
			a++;
			continue;
		}
		break;
	}
	return *a;
}
