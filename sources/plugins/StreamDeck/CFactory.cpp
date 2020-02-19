#include "CFactory.h"
#include "DeviceManager.h"
#include "StreamDeckOriginal.h"

boost::shared_ptr<CDeviceManager> CFactory::createDeviceManager(CConfiguration& configuration)
{
	return boost::make_shared<CStreamDeckOriginal>(configuration);
}