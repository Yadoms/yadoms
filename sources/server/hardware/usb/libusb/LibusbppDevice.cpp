#include "stdafx.h"
#include "LibusbppDevice.h"
#include <shared/Log.h>
#include <codecvt>

namespace hardware
{
   namespace usb
   {
      CLibusbppDevice::CLibusbppDevice(std::shared_ptr<LibUSB::Device> libusbppDevice)
         :m_libusbppDevice(libusbppDevice)
      {
         m_vendorId = m_libusbppDevice->vendorID();
         m_productId = m_libusbppDevice->productID();

         try
         {
            if (!m_libusbppDevice->isOpen())
               m_libusbppDevice->Open(); //TODO trouver une solution pour accéder aux noms sans ouvrir le périph ? (lsusb y arrive, usb-devices aussi...)

            m_serialNumber = wstringToString(m_libusbppDevice->SerialString());

            m_yadomsFriendlyName = wstringToString(m_libusbppDevice->ManufacturerString()) + " - " + wstringToString(m_libusbppDevice->ProductString());
            if (!m_serialNumber.empty())
               m_yadomsFriendlyName += " - " + m_serialNumber;
         }
         catch(const std::exception& e)
         {
            YADOMS_LOG(warning) << "Unable to open USB device " << m_vendorId << ":" << m_productId
               << ", serial number and device name will not be available";
            m_yadomsFriendlyName = std::string("USB device ") + std::to_string(m_vendorId) + ":" + std::to_string(m_productId);
         }

      }

      std::string CLibusbppDevice::wstringToString(const std::wstring& wstring)
      {
         return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wstring);
      }

      std::string CLibusbppDevice::yadomsConnectionId() const
      {
         return std::to_string(vendorId()) + ";" + std::to_string(productId()) + ";" + serialNumber(); //TODO or endPoint number ? Device address ?
      }

      std::string CLibusbppDevice::yadomsFriendlyName() const
      {
         return m_yadomsFriendlyName;
      }

      int CLibusbppDevice::vendorId() const
      {
         return m_vendorId;
      }

      int CLibusbppDevice::productId() const
      {
         return m_productId;
      }

      std::string CLibusbppDevice::serialNumber() const
      {
         return m_serialNumber;
      }
   } // namespace usb
} // namespace hardware
