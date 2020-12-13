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

uint16_t CDeviceManagerHelper::stringToUnsignedShort(std::string& value)
{
	return static_cast<uint16_t>(std::stoi(value, nullptr, 16));
}

CStreamDeckEnum::EStreamDeckType CDeviceManagerHelper::getDeviceModel(uint16_t& productId)
{
	switch (productId)
	{
	case(StreamDeckOriginalPoductId):
		return CStreamDeckEnum::EStreamDeckType::kOriginal;
	case(StreamDeckOriginalV2PoductId):
		return CStreamDeckEnum::EStreamDeckType::kOriginalV2;
	case(StreamDeckMiniPoductId):
		return CStreamDeckEnum::EStreamDeckType::kMini;
	case(StreamDeckXLPoductId):
		return CStreamDeckEnum::EStreamDeckType::kXl;
	default:
		throw std::runtime_error("Unknown device model");
	}
}

int CDeviceManagerHelper::getDeviceKeyCols(uint16_t& productId)
{
	switch (productId)
	{
	case(StreamDeckOriginalPoductId):
		return 5;
	case(StreamDeckOriginalV2PoductId):
		return 5;
	case(StreamDeckMiniPoductId):
		return 3;
	case(StreamDeckXLPoductId):
		return 8;
	default:
		throw std::runtime_error("Unknown device model");
	}
}

int CDeviceManagerHelper::getDeviceKeyRows(uint16_t& productId)
{
	switch (productId)
	{
	case(StreamDeckOriginalPoductId):
		return 3;
	case(StreamDeckOriginalV2PoductId):
		return 3;
	case(StreamDeckMiniPoductId):
		return 2;
	case(StreamDeckXLPoductId):
		return 4;
	default:
		throw std::runtime_error("Unknown device model");
	}
}

int CDeviceManagerHelper::getDeviceKeyCount(uint16_t& productId)
{
	switch (productId)
	{
	case(StreamDeckOriginalPoductId):
		return 15;
	case(StreamDeckOriginalV2PoductId):
		return 15;
	case(StreamDeckMiniPoductId):
		return 6;
	case(StreamDeckXLPoductId):
		return 32;
	default:
		throw std::runtime_error("Unknown device model");
	}
}

std::vector<std::string> CDeviceManagerHelper::buildKeys(const int cols, const int rows)
{
	std::vector<std::string> keys;

	for (auto i = 0; i < cols * rows; i++)
	{
		keys.emplace_back("KEY #" + std::to_string(i));
	}
	return keys;
}

boost::shared_ptr<CUsbDeviceInformation> CDeviceManagerHelper::getDeviceInformation(CConfiguration& configuration)
{
	auto deviceInformation = boost::make_shared<CUsbDeviceInformation>();
	auto usbDevice = configuration.getUsbDevice();
	auto usbDeviceInformation = splitStringToVectorOfString(usbDevice, ";");

	deviceInformation->setVendorId(decimalToHex(usbDeviceInformation[0]));
	deviceInformation->setProductId(decimalToHex(usbDeviceInformation[1]));
	deviceInformation->setSerialNumber(usbDeviceInformation[2]);
	deviceInformation->setDeviceModel(getDeviceModel(deviceInformation->getProductId()));
	deviceInformation->setKeyCols(getDeviceKeyCols(deviceInformation->getProductId()));
	deviceInformation->setKeyRows(getDeviceKeyRows(deviceInformation->getProductId()));
	deviceInformation->setKeyCount(getDeviceKeyCount(deviceInformation->getProductId()));
	
	return deviceInformation;

}

unsigned char CDeviceManagerHelper::integerToHex(int& value)
{
	boost::cnv::cstream converter;
	return boost::convert<uint16_t>(std::to_string(value), converter(std::showbase)(std::uppercase)(std::dec), 0);
}

int CDeviceManagerHelper::getKeyIndex(std::string& key)
{
	return std::atoi(key.substr(5).c_str());
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


std::string CDeviceManagerHelper::getDeviceModelAsAString(uint16_t& productId)
{
	switch (productId)
	{
	case(StreamDeckOriginalPoductId):
		return "Original";
	case(StreamDeckOriginalV2PoductId):
		return "Original V2";
	case(StreamDeckMiniPoductId):
		return "Mini";
	case(StreamDeckXLPoductId):
		return "Xl";
	default:
		throw std::runtime_error("Unknown device model");
	}
}