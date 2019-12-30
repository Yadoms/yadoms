#pragma once

namespace hardware
{
   namespace usb
   {
      class IDevice
      {
      public:
         virtual ~IDevice() = default;

         virtual std::string yadomsConnectionId() const = 0;
         virtual std::string yadomsFriendlyName() const = 0;
         virtual int vendorId() const = 0;
         virtual int productId() const = 0;
         virtual std::string serialNumber() const = 0;
      };
   } // namespace usb
} // namespace hardware
