#include "stdafx.h"
#include "../DevicesListerFactory.h"
#include "LibusbppDevicesLister.h"


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<IDevicesLister> CDevicesListerFactory::createDeviceLister()
      {
         return boost::make_shared<CLibusbppDevicesLister>();
      }
   } // namespace usb
} // namespace hardware
