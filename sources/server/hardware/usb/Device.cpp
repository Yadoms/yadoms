#include "stdafx.h"
#include "Device.h"
#include <codecvt>

namespace hardware
{
   namespace usb
   {
      CDevice::CDevice(std::shared_ptr<LibUSB::Device> libusbppDevice)
         :m_libusbppDevice(libusbppDevice)
      {
      }

      std::string CDevice::wstringToString(const std::wstring& wstring)
      {
         return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wstring);
      }

      std::string CDevice::yadomsConnectionId() const
      {
         return std::to_string(vendorId()) + ";" + std::to_string(productId()) + ";" + serialNumber();
      }

      std::string CDevice::yadomsFriendlyName() const
      {
         //TODO
         //TODO need to open device, but doesn't work under windows return wstringToString(m_libusbppDevice->ProductString());
         return "Stream Deck XL - CL30I1A03593";
      }

      int CDevice::vendorId() const
      {
         //TODO
         return m_libusbppDevice->vendorID();
         return 0x0fd9;
      }

      int CDevice::productId() const
      {
         //TODO
         return m_libusbppDevice->productID();
         return 0x0060;
      }

      std::string CDevice::serialNumber() const
      {
         //TODO
//TODO need to open device, but doesn't work under windows          return wstringToString(m_libusbppDevice->SerialString());
         return "AL25I1C03149";
      }
   } // namespace usb
} // namespace hardware
