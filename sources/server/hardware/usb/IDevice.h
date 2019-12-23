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
         virtual std::string friendlyName() const = 0;
         virtual shared::CDataContainer toContainer() const = 0;
      };
   } // namespace usb
} // namespace hardware
