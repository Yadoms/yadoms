#include "stdafx.h"
#include "WinapiDevice.h"
#include <SetupAPI.h>

#include <utility>

namespace hardware
{
   namespace usb
   {
      CWinapiDevice::CWinapiDevice(std::string devicePath,
                                   int vid,
                                   int pid,
                                   std::string serialNumber,
                                   boost::shared_ptr<const shared::CDataContainer> windowsProperties)
         : m_devicePath(std::move(devicePath)),
           m_vid(vid),
           m_pid(pid),
           m_serialNumber(std::move(serialNumber)),
           m_windowsProperties(std::move(windowsProperties))
      {
      }

      std::string CWinapiDevice::nativeConnectionString() const
      {
         return m_devicePath;
      }

      std::string CWinapiDevice::friendlyName() const
      {
         return m_windowsProperties->getWithDefault<std::string>(std::to_string(SPDRP_FRIENDLYNAME), std::string());
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

      boost::shared_ptr<const shared::CDataContainer> CWinapiDevice::allParameters() const
      {
         return m_windowsProperties;
      }

      std::string CWinapiDevice::getWindowsProperty(unsigned int spdrpPropertyId,
                                                    bool throwIfNotFound) const
      {
         try
         {
            return m_windowsProperties->get<std::string>(std::to_string(spdrpPropertyId));
         }
         catch (const shared::exception::CInvalidParameter&)
         {
            if (throwIfNotFound)
               throw;
            return std::string();
         }
      }
   } // namespace usb
} // namespace hardware
