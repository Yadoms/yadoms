#pragma once
#include "IDeviceState.h"
#include "INotificationSender.h"
#include "Configuration.h"

class CFactory
{
public:
   CFactory() = default;
   virtual ~CFactory() = default;

   static boost::shared_ptr<IDeviceState> createDeviceState(CConfiguration& lametricConfiguration);
   static boost::shared_ptr<INotificationSender> createNotificationSender(CConfiguration& lametricConfiguration);
};
