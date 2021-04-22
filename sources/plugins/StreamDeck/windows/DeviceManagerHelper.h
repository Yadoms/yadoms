#pragma once
#include "../Configuration.h"
#include "shared/plugin/yPluginApi/historization/IHistorizable.h"
#include "../Entities/UsbDeviceInformation.h"

class CDeviceManagerHelper
{
public:
	explicit CDeviceManagerHelper();

	virtual ~CDeviceManagerHelper() = default;

	static uint16_t decimalToHex(std::string decimalValue);

	static uint16_t stringToUnsignedShort(std::string& value);

	static CStreamDeckEnum::EStreamDeckType getDeviceModel(uint16_t& productId);

	static std::string findUsbDeviceId(std::string& value, const std::string& identifierToFind);

	static std::string getSerialNumber(std::string& value);

	static std::vector<std::string> buildKeys(int cols, int rows);

	static int getDeviceKeyCols(uint16_t& productId);

	static int getDeviceKeyRows(uint16_t& productId);

	static int getDeviceKeyCount(uint16_t& productId);

	static boost::shared_ptr<CUsbDeviceInformation> getDeviceInformation(CConfiguration& configuration);

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

	template <typename K, typename V>
	static V secondValueFromPair(std::pair<K, V> const& pair)
	{
		return pair.second;
	}

	static std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> mapToHistorizableVector(
		std::map<int, boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& map);
	
	static std::vector<unsigned char> unsignedCharToVectorOfUnsignedChar(unsigned char* input, int offset, int size);

	enum EHidError
	{
		kFalse,
		kTrue = -1,

	};

	static std::string getDeviceModelAsAString(uint16_t& productId);
private:

	static const uint16_t StreamDeckVendorId;
	static const uint16_t StreamDeckOriginalPoductId;
	static const uint16_t StreamDeckOriginalV2PoductId;
	static const uint16_t StreamDeckMiniPoductId;
	static const uint16_t StreamDeckXLPoductId;
};

