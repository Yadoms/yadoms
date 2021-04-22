#pragma once
#include "IDevicesLister.h"

namespace hardware
{
   namespace usb
   {
      class CDevicesListerFactory
      {
      public:
         virtual ~CDevicesListerFactory() = default;

         static boost::shared_ptr<IDevicesLister> createDeviceLister();
      };
   } // namespace usb
} // namespace hardware
