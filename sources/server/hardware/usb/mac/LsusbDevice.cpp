#include "stdafx.h"
#include "LsusbDevice.h"
#include "LsusbCall.h"
#include <shared/Log.h>
#include <regex>

namespace hardware
{
namespace usb
{
CLsusbDevice::CLsusbDevice(int vendorId,
                           int productId,
                           const std::string &name,
                           const std::stirng &serial)
    : m_vendorId(vendorId),
      m_productId(productId),
      m_yadomsFriendlyName(name),
      m_serialNumber(serial)
{
}

std::string CLsusbDevice::nativeConnectionString() const
{
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
