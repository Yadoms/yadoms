#include "UsbService.h"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>

CUsbService::CUsbService() = default;
const uint16_t CUsbService::StreamDeckVendorId = 0x0FD9;

std::list<std::shared_ptr<LibUSB::Device>> CUsbService::findDevice(CConfiguration& configuration)
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
std::list<std::shared_ptr<LibUSB::Device>> CUsbService::getStreamDeckDevices()
{
	auto foundedDevice = LibUSB::LibUSB::FindAllDevices();
	std::list<std::shared_ptr<LibUSB::Device>> deviceList;
	
	for (const auto device : foundedDevice)
	{

		if(StreamDeckVendorId == decimalToHex(std::to_string(device->vendorID())))
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
std::vector<std::string> CUsbService::splitStringToVectorOfString(std::string& wordToSplit, const std::string separator)
{
	std::vector<std::string> words;
	return split(words, wordToSplit, boost::is_any_of(separator), boost::token_compress_on);
}

uint16_t CUsbService::decimalToHex(std::string& decimalValue) 
{
	boost::cnv::cstream converter;
	return boost::convert<uint16_t>(decimalValue, converter(std::showbase)(std::uppercase)(std::dec),0);
}
