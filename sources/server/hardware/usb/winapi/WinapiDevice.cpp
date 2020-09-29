#include "stdafx.h"
#include "WinapiDevice.h"

namespace hardware
{
   namespace usb
   {
      CWinapiDevice::CWinapiDevice(const std::string& devicePath,
                                   int vid,
                                   int pid,
                                   const std::string& serialNumber,
                                   boost::shared_ptr<const std::map<unsigned int, std::string>> windowsPropertyMap)
         : m_devicePath(devicePath),
           m_vid(vid),
           m_pid(pid),
           m_serialNumber(serialNumber),
           m_windowsPropertyMap(windowsPropertyMap)
      {
      }

      std::string CWinapiDevice::nativeConnectionString() const
      {
         return m_devicePath;
      }

      std::string CWinapiDevice::yadomsFriendlyName() const
      {
         return m_windowsPropertyMap->at(SPDRP_FRIENDLYNAME);
      }

      int CWinapiDevice::vendorId() const
      {
         return m_vid;
      }

      int CWinapiDevice::productId() const
      {
         return m_pid;
      }

      std::string CWinapiDevice::serialNumber() const
      {
         return m_serialNumber;
      }

      std::string CWinapiDevice::getWindowsProperty(unsigned int spdrpPropertyId,
                                                    bool throwIfNotFound) const
      {
         try
         {
            return m_windowsPropertyMap->at(spdrpPropertyId);
         }
         catch (const std::out_of_range&)
         {
            if (throwIfNotFound)
               throw;
            return std::string();
         }
      }
   } // namespace usb
} // namespace hardware
