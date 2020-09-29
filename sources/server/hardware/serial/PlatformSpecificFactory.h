#pragma once
#include "ISerialPortsLister.h"
#include "hardware/usb/IDevicesLister.h"

namespace hardware
{
   namespace serial
   {
      class CPlatformSpecificFactory
      {
      public:
         virtual ~CPlatformSpecificFactory() = default;

         static boost::shared_ptr<ISerialPortsLister> createSerialPortsLister(boost::shared_ptr<usb::IDevicesLister> usbDeviceListers);
      };


   } // namespace serial

} // namespace hardware
