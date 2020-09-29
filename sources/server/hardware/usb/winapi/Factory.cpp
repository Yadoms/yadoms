#include "stdafx.h"
#include "../Factory.h"
#include "WinapiDevicesLister.h"
#include "WinApiSerialPortAdapterDevice.h"


namespace hardware
{
   namespace usb
   {
      boost::shared_ptr<IDevicesLister> CFactory::createDeviceLister()
      {
         return boost::make_shared<CWinapiDevicesLister>();
      }

      boost::shared_ptr<ISerialPortAdapterDevice> CFactory::createSerialPortAdapterDevice(
         boost::shared_ptr<const IDevice> usbDevice)
      {
         return boost::make_shared<CWinApiSerialPortAdapterDevice>(usbDevice);
      }
   } // namespace usb
} // namespace hardware
