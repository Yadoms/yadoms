#include "DeviceManagerHelper.h"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>

CDeviceManagerHelper::CDeviceManagerHelper()
{
}

std::vector<std::string> CDeviceManagerHelper::splitStringToVectorOfString(const std::string& wordToSplit,
	const std::string& separator)
{
	std::vector<std::string> words;
	return split(words, wordToSplit, boost::is_any_of(separator), boost::token_compress_on);
}

uint16_t CDeviceManagerHelper::decimalToHex(std::string& decimalValue)
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