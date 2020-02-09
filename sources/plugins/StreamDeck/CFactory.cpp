#include "CFactory.h"
#include "DeviceManager.h"

boost::shared_ptr<IDeviceManager> CFactory::createDeviceManager(CConfiguration& configuration)
{
	return boost::make_shared<CDeviceManager>(configuration);
}
