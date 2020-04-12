#pragma once
#include "Configuration.h"
#include "DeviceManager.h"

class CFactory
{
public:

	CFactory() = default;
	~CFactory() = default;
	// TODO : create IDeviceManager Interface
	static boost::shared_ptr<CDeviceManager> createDeviceManager(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int mainEvtPortDataReceived);
};
