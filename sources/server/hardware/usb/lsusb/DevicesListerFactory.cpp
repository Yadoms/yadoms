#include "stdafx.h"
#include "../DevicesListerFactory.h"
#include "LsusbDevicesLister.h"


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<IDevicesLister> CDevicesListerFactory::createDeviceLister()
      {
         return boost::make_shared<CLsusbDevicesLister>();
      }
   } // namespace usb
} // namespace hardware
