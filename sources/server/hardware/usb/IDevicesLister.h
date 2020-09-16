#pragma once
#include "IDevice.h"

namespace hardware
{
   namespace usb
   {
      class IDevicesLister
      {
      public:
         virtual ~IDevicesLister() = default;

         virtual std::vector<boost::shared_ptr<IDevice>> listUsbDevices() = 0;
      };
   } // namespace usb
} // namespace hardware
