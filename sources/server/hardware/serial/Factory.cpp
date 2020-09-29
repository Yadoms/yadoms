#include "stdafx.h"
#include "Factory.h"
#include "SerialPortsManager.h"
#include "SerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsManager> CFactory::createSerialPortsManager(
         boost::shared_ptr<usb::IDevicesLister> usbDeviceListers)
      {
         return boost::make_shared<CSerialPortsManager>(boost::make_shared<CSerialPortsLister>(usbDeviceListers));
      }
   } // namespace serial
} // namespace hardware
