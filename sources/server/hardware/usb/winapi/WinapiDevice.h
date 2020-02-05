#pragma once
#include "../IDevice.h"

namespace hardware
{
   namespace usb
   {
      class CWinapiDevice : public IDevice
      {
      public:
         explicit CWinapiDevice(const std::string& deviceName,
                                const std::string& devicePath,
                                int vid,
                                int pid,
                                const std::string& serial);
         virtual ~CWinapiDevice() = default;

         // IDevice implementation
         std::string nativeConnectionString() const override;
         std::string yadomsFriendlyName() const override;
         int vendorId() const override;
         int productId() const override;
         std::string serialNumber() const override;
         // [END] IDevice implementation

      private:
         const std::string m_deviceName;
         const std::string m_devicePath;
         const int m_vid;
         const int m_pid;
         const std::string m_serial;
      };
   } // namespace usb
} // namespace hardware
