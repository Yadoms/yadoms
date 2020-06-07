#include "stdafx.h"
#include "../DevicesListerFactory.h"
#include "SystemProfilerDevicesLister.h"


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<IDevicesLister> CDevicesListerFactory::createDeviceLister()
      {
         return boost::make_shared<CSystemProfilerDevicesLister>();
      }
   } // namespace usb
} // namespace hardware
