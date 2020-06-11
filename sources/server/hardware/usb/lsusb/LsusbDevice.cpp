#include "stdafx.h"
#include "LsusbDevice.h"
#include "LsusbCall.h"
#include <shared/Log.h>
#include <regex>

namespace hardware
{
namespace usb
{
std::string CLsusbDevice::idToHexString(unsigned int value)
{
   std::stringstream ss;
   ss << std::uppercase
      << std::setfill('0') << std::setw(2)
      << std::hex << value;
   return ss.str();
}

CLsusbDevice::CLsusbDevice(int vendorId,
                           int productId,
                           const std::string &name)
    : m_vendorId(vendorId),
      m_productId(productId),
      m_yadomsFriendlyName(name)
{
   try
   {
      std::vector<std::string> args;
      args.push_back("-d " + idToHexString(m_vendorId) + ":" + idToHexString(m_productId));
      args.push_back("-v");

      CLsusbCall lsUsbCall(args);
      const auto lines = lsUsbCall.execute(true);

      for (const auto &line : lines)
      {
         try
         {
            std::smatch matches;
            if (!std::regex_search(line,
                                   matches,
                                   std::regex(std::string("^ *iSerial *[[:digit:]]* ([[:xdigit:]]*)$"))) ||
                matches.size() != 2)
               continue;

            m_serialNumber = matches[1];
            return;
         }
         catch (const std::exception &e)
         {
            YADOMS_LOG(warning) << "Unable to access device " << e.what();
         }
      }
   }
   catch (const std::exception &e)
   {
      YADOMS_LOG(warning) << "Unable to read USB device information, " << e.what();
   }
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
