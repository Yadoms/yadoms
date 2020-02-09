#pragma once
#include "DeviceManagerHelper.h"
#include <libusbpp.hpp>
class IDeviceManager
{
public:

	// ----------------------------------------
	/// \brief	        Destructor
	// ---------------------------------------- 
	virtual ~IDeviceManager() = default;

	virtual std::list<std::shared_ptr<LibUSB::Device>> findDevice() const = 0;

	virtual std::list<std::shared_ptr<LibUSB::Device>> getStreamDeckDevices() const = 0;

	virtual boost::shared_ptr<UsbDeviceInformation> getDeviceInformation() = 0;

};

