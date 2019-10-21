#pragma once
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "LametricConfiguration.h"

class IUrlManager
{
public:
   virtual ~IUrlManager() = default;

   virtual shared::CDataContainer getDeviceState() = 0;

   virtual shared::CDataContainer getWifiState() = 0;

   virtual shared::CDataContainer getBluetoothState() = 0;

   virtual shared::CDataContainer getAudioState() = 0;
};
