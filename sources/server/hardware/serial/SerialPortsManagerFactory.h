#pragma once
#include "ISerialPortsManager.h"
#include "hardware/usb/IDevicesLister.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPortsManagerFactory
      {
      public:
         CSerialPortsManagerFactory() = delete;
         virtual ~CSerialPortsManagerFactory() = delete;

         static boost::shared_ptr<ISerialPortsManager> createSerialPortsManager(boost::shared_ptr<usb::IDevicesLister> usbDeviceListers);
      };
   } // namespace serial
} // namespace hardware
