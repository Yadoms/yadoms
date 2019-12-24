#pragma once
#include <shared/DataContainer.h>

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
         virtual shared::CDataContainer toContainer() const = 0;//TODO utile ?
      };
   } // namespace usb
} // namespace hardware
