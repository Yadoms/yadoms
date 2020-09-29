#pragma once

namespace hardware
{
   namespace usb
   {
      class ISerialPortAdapterDevice//TODO virer
      {
      public:
         virtual ~ISerialPortAdapterDevice() = default;

         virtual std::string connectionPath() const = 0;
      };
   } // namespace usb
} // namespace hardware
