#include "stdafx.h"
#include "../DevicesListerFactory.h"
#include "WinapiDevicesLister.h"


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<IDevicesLister> CDevicesListerFactory::createDeviceLister()
      {
         return boost::make_shared<CWinapiDevicesLister>();
      }
   } // namespace usb
} // namespace hardware
