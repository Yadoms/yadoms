#pragma once
#include "../IDevice.h"
#include <SetupAPI.h>

namespace hardware
{
   namespace usb
   {
      class CWinapiDevice : public IDevice
      {
      public:
         explicit CWinapiDevice(const std::string& devicePath,
                                int vid,
                                int pid,
                                const std::string& serialNumber,
                                boost::shared_ptr<const std::map<unsigned int, std::string>> windowsPropertyMap);
         virtual ~CWinapiDevice() = default;

         // IDevice implementation
         std::string nativeConnectionString() const override;
         std::string yadomsFriendlyName() const override;
         int vendorId() const override;
         int productId() const override;
         std::string serialNumber() const override;
         // [END] IDevice implementation

         /// \brief Get Windows specific property from device registry property codes
         /// \param spdrpPropertyId Device registry property codes (from 0 to SPDRP_MAXIMUM_PROPERTY)
         /// \param throwIfNotFound Will throw if property not found, else returns empty string
         /// \note see SetupAPI.h
         std::string getWindowsProperty(unsigned int spdrpPropertyId,
                                        bool throwIfNotFound = false) const;

      private:
         const std::string m_devicePath;
         const int m_vid;
         const int m_pid;
         const std::string m_serialNumber;
         boost::shared_ptr<const std::map<unsigned int, std::string>> m_windowsPropertyMap;
      };
   } // namespace usb
} // namespace hardware
