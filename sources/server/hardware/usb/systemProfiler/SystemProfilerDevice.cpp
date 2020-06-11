#include "stdafx.h"
#include "SystemProfilerDevice.h"
#include "SystemProfilerCall.h"
#include <shared/Log.h>
#include <regex>

namespace hardware
{
namespace usb
{
std::string CSystemProfilerDevice::idToHexString(unsigned int value)
{
   std::stringstream ss;
   ss << std::uppercase
      << std::setfill('0') << std::setw(2)
      << std::hex << value;
   return ss.str();
}

CSystemProfilerDevice::CSystemProfilerDevice(int vendorId,
                           int productId,
                           const std::string &name,
                           const std::string &serialNumber)
    : m_vendorId(vendorId),
      m_productId(productId),
      m_yadomsFriendlyName(name),
      m_serialNumber(serialNumber)
{
   
}

std::string CSystemProfilerDevice::nativeConnectionString() const
{
   return std::to_string(vendorId()) + ";" + std::to_string(productId()) + ";" + serialNumber(); //TODO or endPoint number ? Device address ?
}

std::string CSystemProfilerDevice::yadomsFriendlyName() const
{
   return m_yadomsFriendlyName;
}

int CSystemProfilerDevice::vendorId() const
{
   return m_vendorId;
}

int CSystemProfilerDevice::productId() const
{
   return m_productId;
}

std::string CSystemProfilerDevice::serialNumber() const
{
   return m_serialNumber;
}
} // namespace usb
} // namespace hardware
