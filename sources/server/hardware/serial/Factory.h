#pragma once
#include "ISerialPortsManager.h"
#include "hardware/usb/IDevicesLister.h"
#include "database/ISerialPortRequester.h"

namespace hardware
{
   namespace serial
   {
      class CFactory
      {
      public:
         CFactory() = delete;
         virtual ~CFactory() = delete;

         static boost::shared_ptr<ISerialPortsManager> createSerialPortsManager(
            boost::shared_ptr<usb::IDevicesLister> usbDeviceListers,
            boost::shared_ptr<database::ISerialPortRequester> serialPortDatabaseRequester);
      };
   } // namespace serial
} // namespace hardware
