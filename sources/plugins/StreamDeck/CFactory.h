#pragma once
#include "Configuration.h"
#include "DeviceManager.h"
#include "DeviceManagerHelper.h"

class CFactory
{
public:

	CFactory() = default;
	~CFactory() = default;
	// TODO : create IDeviceManager Interface
	static boost::shared_ptr<CDeviceManager> createDeviceManager(CConfiguration& configuration,
	                                                             boost::shared_ptr<UsbDeviceInformation>&
	                                                             deviceInformation,
	                                                             shared::event::CEventHandler& mainEventHandler,
	                                                             int mainEvtPortDataReceived);
};
