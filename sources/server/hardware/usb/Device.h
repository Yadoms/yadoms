#pragma once
#include "IDevice.h"

namespace hardware
{
   namespace usb
   {
      class CDevice : public IDevice
      {
      public:
         virtual ~CDevice() = default;

         // IDevice implementation
         std::string yadomsConnectionId() const override;
         std::string friendlyName() const override;
         shared::CDataContainer toContainer() const override;
         // [END] IDevice implementation
      };
   } // namespace usb
} // namespace hardware
