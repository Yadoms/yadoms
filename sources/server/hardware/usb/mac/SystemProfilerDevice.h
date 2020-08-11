#pragma once
#include "../IDevice.h"

namespace hardware
{
namespace usb
{
class CSystemProfilerDevice : public IDevice
{
public:
   explicit CSystemProfilerDevice(int vendorId,
                                  int productId,
                                  const std::string &name,
                                  const std::string &serialNumber);
    
   virtual ~CSystemProfilerDevice() = default;

   // IDevice implementation
   std::string nativeConnectionString() const override;
   std::string yadomsFriendlyName() const override;
   int vendorId() const override;
   int productId() const override;
   std::string serialNumber() const override;
   // [END] IDevice implementation

private:
   static std::string idToHexString(unsigned int value);

   int m_vendorId;
   int m_productId;
   std::string m_yadomsFriendlyName;
   std::string m_serialNumber;
};
} // namespace usb
} // namespace hardware
