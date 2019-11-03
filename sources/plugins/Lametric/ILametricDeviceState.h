#pragma once

#include <shared/DataContainer.h>
#include "NotificationProperties.h"

class ILametricDeviceState
{
public:
   virtual ~ILametricDeviceState() = default;

   virtual shared::CDataContainer getDeviceState() = 0;

   virtual shared::CDataContainer getWifiState() = 0;

   virtual shared::CDataContainer getBluetoothState() = 0;

   virtual shared::CDataContainer getAudioState() = 0;
};
