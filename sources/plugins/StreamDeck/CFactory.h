#pragma once
#include "Configuration.h"
#include "DeviceManager.h"
class CFactory
{
public:
	
   CFactory() = default;
   ~CFactory() = default;

   static boost::shared_ptr<CDeviceManager> CFactory::createDeviceManager(CConfiguration& configuration);
};

