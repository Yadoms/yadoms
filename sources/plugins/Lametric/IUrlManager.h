#pragma once
#include <shared/DataContainer.h>
#include "LametricConfiguration.h"

class IUrlManager
{
public:
   virtual ~IUrlManager() = default;

   virtual shared::CDataContainer getDeviceState() const = 0;

   virtual shared::CDataContainer getWifiState() const = 0;

   virtual shared::CDataContainer getBluetoothState() const = 0;

   virtual shared::CDataContainer getAudioState() const = 0;

   virtual void displayText(const std::string& text) const = 0;
};
