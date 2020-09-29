#pragma once
#include "ISerialPortsManager.h"
#include "hardware/usb/IDevicesLister.h"

namespace hardware
{
   namespace serial
   {
      class CFactory
      {
      public:
         CFactory() = delete;
         virtual ~CFactory() = delete;

         static boost::shared_ptr<ISerialPortsManager> createSerialPortsManager(boost::shared_ptr<usb::IDevicesLister> usbDeviceListers);
      };
   } // namespace serial
} // namespace hardware
