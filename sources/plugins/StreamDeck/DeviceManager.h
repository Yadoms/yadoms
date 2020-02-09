#pragma once
#include <libusbpp.hpp>
#include "Configuration.h"

struct UsbDeviceInformation
{
	uint16_t vendorID;
	uint16_t productID;
	std::string serialNumber;
};
class CDeviceManager final
{
public:
	CDeviceManager();
	virtual ~CDeviceManager() = default;

	static std::list<std::shared_ptr<LibUSB::Device>> findDevice(CConfiguration& configuration);
	static std::list<std::shared_ptr<LibUSB::Device>> getStreamDeckDevices();

	static std::vector<std::string> splitStringToVectorOfString(const std::string& wordToSplit, const std::string& separator);

	static uint16_t decimalToHex(std::string& decimalValue);

	static boost::shared_ptr<UsbDeviceInformation> getDeviceInformation(CConfiguration& configuration);
private:
	static const uint16_t StreamDeckVendorId;
	static std::string getOsName();
	static uint16_t stringToUnsignedShort(std::string& value);
	static std::string findUsbDeviceId(std::string& value, const std::string& identifierToFind);
	static std::string getSerialNumber(std::string& value);
};

 