#pragma once
#include "IDeviceManager.h"
#include "Configuration.h"
class CFactory
{
public:
	
   CFactory() = default;
   ~CFactory() = default;

   static boost::shared_ptr<IDeviceManager> createDeviceManager(CConfiguration& configuration);
};

