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
         //TODO
         //TODO need to open device, but doesn't work under windows return wstringToString(m_libusbppDevice->ProductString());
         return "Stream Deck XL - CL30I1A03593";
      }

      int CLibusbppDevice::vendorId() const
      {
         //TODO
         return m_libusbppDevice->vendorID();
         return 0x0fd9;
      }

      int CLibusbppDevice::productId() const
      {
         //TODO
         return m_libusbppDevice->productID();
         return 0x0060;
      }

      std::string CLibusbppDevice::serialNumber() const
      {
         //TODO
//TODO need to open device, but doesn't work under windows          return wstringToString(m_libusbppDevice->SerialString());
         return "AL25I1C03149";
      }
   } // namespace usb
} // namespace hardware
