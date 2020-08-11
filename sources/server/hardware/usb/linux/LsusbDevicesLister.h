#pragma once
#include "../IDevicesLister.h"

namespace hardware
{
   namespace usb
   {
      class CLsusbDevicesLister : public IDevicesLister
      {
      public:
         virtual ~CLsusbDevicesLister() = default;

         // IDevicesLister implementation
         std::vector<boost::shared_ptr<IDevice>> listUsbDevices() override;
         // [END] IDevicesLister implementation
      };
   } // namespace usb
} // namespace hardware
