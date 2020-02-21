#pragma once
#include "../IDevice.h"

namespace hardware
{
namespace usb
{
class CLsusbDevice : public IDevice
{
public:
   explicit CLsusbDevice(int vendorId,
                         int productId,
                         const std::string &name,
                         const std::stirng &serial);
   virtual ~CLsusbDevice() = default;

   // IDevice implementation
   std::string nativeConnectionString() const override;
   std::string yadomsFriendlyName() const override;
   int vendorId() const override;
   int productId() const override;
   std::string serialNumber() const override;
   // [END] IDevice implementation

private:
   const int m_vendorId;
   const int m_productId;
   const std::string m_yadomsFriendlyName;
   const std::string m_serialNumber;
};
} // namespace usb
} // namespace hardware
