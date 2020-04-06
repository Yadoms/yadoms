#pragma once
#include "Configuration.h"

struct UsbDeviceInformation
{
	uint16_t vendorID{};
	uint16_t productID{};
	std::string serialNumber;
	std::string deviceName = "Stream Deck";
	std::string deviceModel;
	std::string deviceType;
	int keyCols;
	int keyRows;
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

	static std::vector<std::string> buildKeys(int cols, int rows);

	static int getDeviceKeyCols(uint16_t& vendorId, uint16_t& productId);

	static int getDeviceKeyRows(uint16_t& vendorId, uint16_t& productId);

	static boost::shared_ptr<UsbDeviceInformation> getDeviceInformation(CConfiguration& configuration);

	static unsigned char integerToHex(int& value);

	static int getKeyIndex(std::string& key);

	template <typename T>
	static std::pair<bool, int> findInVector(const std::vector<T>& vecOfElements, const T& element)
	{
		std::pair<bool, int> result;

		// Find given element in vector
		auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

		if (it != vecOfElements.end())
		{
			result.second = distance(vecOfElements.begin(), it);
			result.first = true;
		}
		else
		{
			result.first = false;
			result.second = -1;
		}

		return result;
	}
	static std::vector<unsigned char> unsignedCharToVectorOfUnsignedChar(unsigned char* input, int offset, int size);
private:
	
	static const uint16_t StreamDeckVendorId;
	static const uint16_t StreamDeckOriginalPoductId;
	static const uint16_t StreamDeckOriginalV2PoductId;
	static const uint16_t StreamDeckMiniPoductId;
	static const uint16_t StreamDeckXLPoductId;
	
};
