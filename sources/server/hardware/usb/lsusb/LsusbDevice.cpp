#include "stdafx.h"
#include "LsusbDevice.h"
#include <shared/Log.h>
#include <codecvt>

namespace hardware
{
namespace usb
{
CLsusbDevice::CLsusbDevice(int vendorId,
                           int productId,
                           const std::string &name)
    : m_vendorId(vendorId),
      m_productId(productId),
      m_yadomsFriendlyName(name)
{
   /*TODO
   try
   {
      if (!m_libusbppDevice->isOpen())
         m_libusbppDevice->Open(); //TODO trouver une solution pour accéder aux noms sans ouvrir le périph ? (lsusb y arrive, usb-devices aussi...)

      m_serialNumber = (m_libusbppDevice->SerialString().empty() || m_libusbppDevice->SerialString() == L"Not supported.")
                           ? std::string()
                           : wstringToString(m_libusbppDevice->SerialString());

      m_yadomsFriendlyName = wstringToString(m_libusbppDevice->ManufacturerString()) + " - " + wstringToString(m_libusbppDevice->ProductString());
      if (!m_serialNumber.empty())
         m_yadomsFriendlyName += " - " + m_serialNumber;
   }
   catch (const std::exception &e)
   {
      YADOMS_LOG(warning) << "Unable to open USB device " << m_vendorId << ":" << m_productId
                          << ", serial number and device name will not be available";
      m_yadomsFriendlyName = std::string("USB device ") + std::to_string(m_vendorId) + ":" + std::to_string(m_productId);
   }*/
}

std::string CLsusbDevice::yadomsConnectionId() const
{
   //TODO
   return std::to_string(vendorId()) + ";" + std::to_string(productId()) + ";" + serialNumber(); //TODO or endPoint number ? Device address ?
}

std::string CLsusbDevice::yadomsFriendlyName() const
{
   return m_yadomsFriendlyName;
}

int CLsusbDevice::vendorId() const
{
   return m_vendorId;
}

int CLsusbDevice::productId() const
{
   return m_productId;
}

std::string CLsusbDevice::serialNumber() const
{
   return m_serialNumber;
}
} // namespace usb
} // namespace hardware
