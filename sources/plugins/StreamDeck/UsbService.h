#pragma once
#include <libusbpp.hpp>
#include "Configuration.h"

class CUsbService
{
public:
	CUsbService();
	virtual ~CUsbService() = default;

	static std::list<std::shared_ptr<LibUSB::Device>> findDevice(CConfiguration& configuration);

	static std::vector<std::string> splitStringToVectorOfString(std::string& wordToSplit, std::string separator);

	static uint16_t decimalToHex(std::string& decimalValue);
};

