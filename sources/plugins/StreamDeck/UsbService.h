#pragma once
#include <libusbpp.hpp>
#include "Configuration.h"

class CUsbService
{
public:
	CUsbService();
	virtual ~CUsbService() = default;

	static std::list<std::shared_ptr<LibUSB::Device>> findDevice(CConfiguration& configuration);
	static std::list<std::shared_ptr<LibUSB::Device>> getStreamDeckDevices();

	static std::vector<std::string> splitStringToVectorOfString(const std::string& wordToSplit, const std::string& separator);

	static uint16_t decimalToHex(std::string& decimalValue);

private:
	static const uint16_t StreamDeckVendorId;
};

