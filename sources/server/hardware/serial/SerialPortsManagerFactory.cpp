#include "stdafx.h"
#include "SerialPortsManagerFactory.h"
#include "SerialPortsManager.h"
#include "SerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<ISerialPortsManager> CSerialPortsManagerFactory::createSerialPortsManager(
         boost::shared_ptr<usb::IDevicesLister> usbDeviceListers)
      {
         return boost::make_shared<CSerialPortsManager>(boost::make_shared<CSerialPortsLister>(),
                                                        usbDeviceListers);
      }
   } // namespace serial
} // namespace hardware
