#include "stdafx.h"
#include "../Factory.h"
#include "LsusbDevicesLister.h"


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<IDevicesLister> CFactory::createDeviceLister()
      {
         return boost::make_shared<CLsusbDevicesLister>();
      }
   } // namespace usb
} // namespace hardware
