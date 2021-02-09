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

         virtual std::string nativeConnectionString() const = 0;
         virtual std::string friendlyName() const = 0;
         virtual int vendorId() const = 0;
         virtual int productId() const = 0;
         virtual std::string serialNumber() const = 0;
         virtual boost::shared_ptr<const shared::CDataContainer> allParameters() const = 0;
      };
   } // namespace usb
} // namespace hardware
