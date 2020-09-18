#pragma once
#include "IDevice.h"

namespace hardware
{
   namespace usb
   {
      class IDevicesLister
      {
      public:
         enum EDeviceClass
         {
            kSerialPorts,
         };

         virtual ~IDevicesLister() = default;

         virtual std::vector<boost::shared_ptr<IDevice>> listUsbDevices() = 0;
         virtual std::vector<boost::shared_ptr<IDevice>> listUsbDevicesForClass(EDeviceClass deviceClass) = 0;
      };
   } // namespace usb
} // namespace hardware
