#pragma once
#include "../IDevice.h"
#include "../ISerialPortAdapterDevice.h"

namespace hardware
{
   namespace usb
   {
      class CLsusbSerialPortAdapterDevice : public ISerialPortAdapterDevice
      {
      public:
         explicit CLsusbSerialPortAdapterDevice();
         virtual ~CLsusbSerialPortAdapterDevice() = default;

         // ISerialPortAdapterDevice implementation
         std::string connectionPath() const override;
         // [END] ISerialPortAdapterDevice implementation
      };
   } // namespace usb
} // namespace hardware
