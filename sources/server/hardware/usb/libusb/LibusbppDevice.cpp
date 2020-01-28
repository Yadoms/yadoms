#include "stdafx.h"
#include "LibusbppDevice.h"
#include <codecvt>

namespace hardware
{
   namespace usb
   {
      CLibusbppDevice::CLibusbppDevice(std::shared_ptr<LibUSB::Device> libusbppDevice)
         :m_libusbppDevice(libusbppDevice)
      {
      }

      std::string CLibusbppDevice::wstringToString(const std::wstring& wstring)
      {
         return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wstring);
      }

      std::string CLibusbppDevice::yadomsConnectionId() const
      {
         return std::to_string(vendorId()) + ";" + std::to_string(productId()) + ";" + serialNumber();
      }

      std::string CLibusbppDevice::yadomsFriendlyName() const
      {
         //TODO à tester
         return wstringToString(m_libusbppDevice->ProductString());
      }

      int CLibusbppDevice::vendorId() const
      {
         //TODO à tester
         return m_libusbppDevice->vendorID();
      }

      int CLibusbppDevice::productId() const
      {
         //TODO à tester
         return m_libusbppDevice->productID();
      }

      std::string CLibusbppDevice::serialNumber() const
      {
         //TODO à tester
         return wstringToString(m_libusbppDevice->SerialString());
      }
   } // namespace usb
} // namespace hardware
