#include "DeviceManagerHelper.h"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <regex>

const uint16_t CDeviceManagerHelper::StreamDeckVendorId = 0x0fd9;
const uint16_t CDeviceManagerHelper::StreamDeckOriginalPoductId = 0x0060;
const uint16_t CDeviceManagerHelper::StreamDeckOriginalV2PoductId = 0x006d;
const uint16_t CDeviceManagerHelper::StreamDeckMiniPoductId = 0x0063;
const uint16_t CDeviceManagerHelper::StreamDeckXLPoductId = 0x006c;

CDeviceManagerHelper::CDeviceManagerHelper() = default;

std::vector<std::string> CDeviceManagerHelper::splitStringToVectorOfString(const std::string& wordToSplit,
                                                                           const std::string& separator)
{
	std::vector<std::string> words;
	return split(words, wordToSplit, boost::is_any_of(separator), boost::token_compress_on);
}

uint16_t CDeviceManagerHelper::decimalToHex(std::string decimalValue)
{
	boost::cnv::cstream converter;
	return boost::convert<uint16_t>(decimalValue, converter(std::showbase)(std::uppercase)(std::dec), 0);
}


std::string CDeviceManagerHelper::getOsName()
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

uint16_t CDeviceManagerHelper::stringToUnsignedShort(std::string& value)
{
	return static_cast<uint16_t>(std::stoi(value, nullptr, 16));
}

std::string CDeviceManagerHelper::getDeviceModel(uint16_t& vendorId, uint16_t& productId)
{
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalPoductId)
		return "Original";
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalV2PoductId)
		return "Original V2";
	if (vendorId == StreamDeckVendorId && productId == StreamDeckMiniPoductId)
		return "Mini";
	if (vendorId == StreamDeckVendorId && productId == StreamDeckXLPoductId)
		return "XL";
	throw;
}

int CDeviceManagerHelper::getDeviceKeyCols(uint16_t& vendorId, uint16_t& productId)
{
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalPoductId)
		return 5;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalV2PoductId)
		return 5;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckMiniPoductId)
		return 3;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckXLPoductId)
		return 8;
	throw;
}

int CDeviceManagerHelper::getDeviceKeyRows(uint16_t& vendorId, uint16_t& productId)
{
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalPoductId)
		return 3;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalV2PoductId)
		return 3;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckMiniPoductId)
		return 2;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckXLPoductId)
		return 4;
	throw;
}

int CDeviceManagerHelper::getDeviceKeyCount(uint16_t& vendorId, uint16_t& productId)
{
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalPoductId)
		return 15;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckOriginalV2PoductId)
		return 15;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckMiniPoductId)
		return 6;
	if (vendorId == StreamDeckVendorId && productId == StreamDeckXLPoductId)
		return 32;
	throw;
}

std::string CDeviceManagerHelper::findUsbDeviceId(std::string& value, const std::string& identifierToFind)
{
	std::smatch matches;
	const std::regex reg(identifierToFind + "_(\\w+)");
	if (!std::regex_search(value, matches, reg) || matches.empty())
	{
		throw;
	}
	return matches[1].str();
}

std::string CDeviceManagerHelper::getSerialNumber(std::string& value)
{
	const auto serialNumberLenght = 12;
	std::smatch matches;
	const std::regex reg("([[:alnum:]]+)");

	const std::regex_token_iterator<std::string::iterator> rend;
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

std::vector<std::string> CDeviceManagerHelper::buildCoordinates(const int cols, const int rows)
{
	std::vector<std::string> coordinates;

	for (auto i = 0; i < rows; i++)
	{
		for (auto j = 0; j < cols; j++)
		{
			coordinates.push_back("(" + std::to_string(i) + "," + std::to_string(j) + ")");
		}
	}
	return coordinates;
}

std::vector<std::string> CDeviceManagerHelper::buildKeys(const int cols, const int rows)
{
	std::vector<std::string> keys;

	for (auto i = 0; i < cols * rows; i++)
	{
		keys.emplace_back("KEY" + std::to_string(i));
	}
	return keys;
}

boost::shared_ptr<UsbDeviceInformation> CDeviceManagerHelper::getDeviceInformation(CConfiguration& configuration)
{
	auto deviceInformation = boost::make_shared<UsbDeviceInformation>();
	auto usbDevice = configuration.getUsbDevice();
	auto usbDeviceVid = findUsbDeviceId(usbDevice, "vid");
	auto usbDevicePid = findUsbDeviceId(usbDevice, "pid");

	if (getOsName() != "Windows")
	{
		auto usbDeviceInformation = splitStringToVectorOfString(usbDevice, ";");

		deviceInformation->vendorID = decimalToHex(usbDeviceInformation[0]);
		deviceInformation->productID = decimalToHex(usbDeviceInformation[1]);
		deviceInformation->serialNumber = usbDeviceInformation[2];
		deviceInformation->deviceModel = getDeviceModel(deviceInformation->vendorID, deviceInformation->productID);
		deviceInformation->keyCols = getDeviceKeyCols(deviceInformation->vendorID, deviceInformation->productID);
		deviceInformation->keyRows = getDeviceKeyRows(deviceInformation->vendorID, deviceInformation->productID);
		deviceInformation->keyCount = getDeviceKeyCount(deviceInformation->vendorID, deviceInformation->productID);
		return deviceInformation;
	}

	deviceInformation->vendorID = stringToUnsignedShort(usbDeviceVid);
	deviceInformation->productID = stringToUnsignedShort(usbDevicePid);
	deviceInformation->serialNumber = getSerialNumber(usbDevice);
	deviceInformation->deviceModel = getDeviceModel(deviceInformation->vendorID, deviceInformation->productID);
	deviceInformation->keyCols = getDeviceKeyCols(deviceInformation->vendorID, deviceInformation->productID);
	deviceInformation->keyRows = getDeviceKeyRows(deviceInformation->vendorID, deviceInformation->productID);
	deviceInformation->keyCount = getDeviceKeyCount(deviceInformation->vendorID, deviceInformation->productID);
	return deviceInformation;
}

unsigned char CDeviceManagerHelper::integerToHex(int& value)
{
	boost::cnv::cstream converter;
	return boost::convert<uint16_t>(std::to_string(value), converter(std::showbase)(std::uppercase)(std::dec), 0);
}

int CDeviceManagerHelper::getKeyIndex(std::string& key)
{
	return std::atoi(key.substr(3).c_str());
}

std::vector<unsigned char> CDeviceManagerHelper::unsignedCharToVectorOfUnsignedChar(unsigned char* input,
                                                                                    const int offset,
                                                                                    const int size)
{
	return std::vector<unsigned char>(input + offset, input + size);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CDeviceManagerHelper::mapToHistorizableVector(
	std::map<int, boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& map)
{
	std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsAsVector;
	keywordsAsVector.reserve(map.size());

	std::transform(map.begin(), map.end(),
	               back_inserter(keywordsAsVector),
	               &secondValueFromPair<
		               int, boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>);
	return keywordsAsVector;
}
