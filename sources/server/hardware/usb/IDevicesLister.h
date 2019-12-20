#pragma once
#include "IDevice.h"
#include <shared/DataContainer.h>

namespace hardware
{
   namespace usb
   {
      class IDevicesLister
      {
      public:
         virtual ~IDevicesLister() = default;

         virtual std::vector<boost::shared_ptr<IDevice>> fromRequest(const shared::CDataContainer& request) const = 0;
      };
   } // namespace usb
} // namespace hardware
