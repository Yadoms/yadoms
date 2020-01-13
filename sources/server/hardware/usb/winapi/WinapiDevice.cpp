#include "stdafx.h"
#include "WinapiDevice.h"

namespace hardware
{
   namespace usb
   {
      CWinapiDevice::CWinapiDevice(const std::string& deviceName,
                                   const std::string& devicePath,
                                   int vid,
                                   int pid,
                                   const std::string& serial)
         : m_deviceName(deviceName),
           m_devicePath(devicePath),
           m_vid(vid),
           m_pid(pid),
           m_serial(serial)
      {
      }

      std::string CWinapiDevice::yadomsConnectionId() const
      {
         return m_devicePath;
      }

      std::string CWinapiDevice::yadomsFriendlyName() const
      {
         return m_deviceName;
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
         return m_serial;
      }
   } // namespace usb
} // namespace hardware
