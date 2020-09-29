#include "stdafx.h"
#include "../PlatformSpecificFactory.h"
#include "SerialPortsLister.h"


namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsLister> CPlatformSpecificFactory::createSerialPortsLister(boost::shared_ptr<usb::IDevicesLister> usbDeviceListers)
      {
         return boost::make_shared<CSerialPortsLister>(usbDeviceListers);
      }
   } // namespace serial
} // namespace hardware
