#pragma once
#include <shared/DataContainer.h>
#include "NotificationProperties.h"

class IUrlManager
{
public:
   virtual ~IUrlManager() = default;

   virtual shared::CDataContainer getDeviceState() const = 0;

   virtual shared::CDataContainer getWifiState() const = 0;

   virtual shared::CDataContainer getBluetoothState() const = 0;

   virtual shared::CDataContainer getAudioState() const = 0;

   virtual void displayText(const std::string& text,
                            notificationProperties::CNotificationPriority::EPriorityType priorityType,
                            notificationProperties::CNotificationIcon::EIconType iconType) const = 0;
};
