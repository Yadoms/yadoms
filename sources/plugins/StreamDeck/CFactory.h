#pragma once
#include "Configuration.h"
#include "DeviceManager.h"
#include "Entities/UsbDeviceInformation.h"

class CFactory
{
public:

	CFactory() = default;
	~CFactory() = default;
	// TODO : create IDeviceManager Interface
	static boost::shared_ptr<CDeviceManager> createDeviceManager(CConfiguration& configuration,
	                                                             boost::shared_ptr<CUsbDeviceInformation>&
	                                                             deviceInformation,
	                                                             shared::event::CEventHandler& mainEventHandler,
	                                                             int mainEvtPortDataReceived);
};
