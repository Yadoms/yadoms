#pragma once
#include "IDevicesLister.h"
#include "ISerialPortAdapterDevice.h"

namespace hardware
{
   namespace usb
   {
      class CFactory
      {
      public:
         virtual ~CFactory() = default;

         static boost::shared_ptr<IDevicesLister> createDeviceLister();

         static boost::shared_ptr<ISerialPortAdapterDevice> createSerialPortAdapterDevice(boost::shared_ptr<const IDevice> usbDevice); //TODO virer
      };
   } // namespace usb
} // namespace hardware
