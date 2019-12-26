#pragma once
#include "IDevice.h"
#include <libusbpp/Device.hpp>

namespace hardware
{
   namespace usb
   {
      class CDevice : public IDevice
      {
      public:
         explicit CDevice(std::shared_ptr<LibUSB::Device> libusbppDevice);
         virtual ~CDevice() = default;

         // IDevice implementation
         std::string yadomsConnectionId() const override;
         std::string yadomsFriendlyName() const override;
         int vendorId() const override;
         int productId() const override;
         std::string serialNumber() const override;
         // [END] IDevice implementation

      private:
         static std::string wstringToString(const std::wstring& wstring);

         std::shared_ptr<LibUSB::Device> m_libusbppDevice;
      };
   } // namespace usb
} // namespace hardware
