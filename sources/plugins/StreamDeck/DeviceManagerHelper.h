#pragma once

struct UsbDeviceInformation
{
	uint16_t vendorID{};
	uint16_t productID{};
	std::string serialNumber;
	std::string deviceName = "Stream Deck";
	std::string deviceModel;
	std::string deviceType;
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

	static std::string getDeviceModel(uint16_t& vendorId, uint16_t& productId);

	static std::string findUsbDeviceId(std::string& value, const std::string& identifierToFind);
	
	static std::string getSerialNumber(std::string& value);

	static std::vector<std::string> buildCoordinates(int cols, int rows);
	
private:
	
	static const uint16_t StreamDeckVendorId;
	static const uint16_t StreamDeckOriginalPoductId;
	static const uint16_t StreamDeckOriginalV2PoductId;
	static const uint16_t StreamDeckMiniPoductId;
	static const uint16_t StreamDeckXLPoductId;
	
};
