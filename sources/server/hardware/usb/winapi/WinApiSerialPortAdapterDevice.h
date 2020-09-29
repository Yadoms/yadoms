#pragma once
#include "../ISerialPortAdapterDevice.h"
#include "../IDevice.h"

namespace hardware
{
   namespace usb
   {
      class CWinApiSerialPortAdapterDevice final : public ISerialPortAdapterDevice
      {
      public:
         explicit CWinApiSerialPortAdapterDevice(boost::shared_ptr<const IDevice> usbDevice);
         virtual ~CWinApiSerialPortAdapterDevice() = default;

         // ISerialPortAdapterDevice implementation
         std::string connectionPath() const override;
         // [END] ISerialPortAdapterDevice implementation

      private:
         bool isSerialPortAdapter(boost::shared_ptr<const IDevice> device);
      };
   } // namespace usb
} // namespace hardware
