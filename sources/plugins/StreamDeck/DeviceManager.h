#pragma once
#include <libusbpp.hpp>
#include "Configuration.h"
#include <hidapi.h>

class CDeviceManager
{
public:

	CDeviceManager(CConfiguration& configuration);

	virtual ~CDeviceManager() = default;

	void open();

	virtual void reset() = 0;

	virtual void resetKeyStream() = 0;

	virtual void setBrightness(int percent) = 0;

	virtual void setKeyImage(std::string& content, int& keyIndex, std::string& customText) = 0;
	
	virtual void readKeyStates() = 0;
private:

	CConfiguration m_configuration;
	static const uint16_t StreamDeckVendorId;

protected:

	hid_device* m_handle;
};
