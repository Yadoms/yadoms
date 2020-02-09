#pragma once

struct UsbDeviceInformation
{
	uint16_t vendorID{};
	uint16_t productID{};
	std::string serialNumber;
};
class CDeviceManagerHelper
{
public:
	explicit CDeviceManagerHelper();

	virtual ~CDeviceManagerHelper() = default;

	static std::vector<std::string> splitStringToVectorOfString(const std::string& wordToSplit,
	                                                            const std::string& separator);

	static uint16_t decimalToHex(std::string& decimalValue);

	static std::string getOsName();

	static uint16_t stringToUnsignedShort(std::string& value);
};
