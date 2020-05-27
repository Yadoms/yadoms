#include "DeviceState.h"
#include "INotificationSender.h"
#include "NotificationSender.h"
#include "CFactory.h"

boost::shared_ptr<IDeviceState> CFactory::createDeviceState(CConfiguration& lametricConfiguration)
{
   return boost::make_shared<CDeviceState>(lametricConfiguration);
}

boost::shared_ptr<INotificationSender> CFactory::createNotificationSender(CConfiguration& lametricConfiguration)
{
   return boost::make_shared<CNotificationSender>(lametricConfiguration);
}
