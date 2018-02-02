#include "stdafx.h"
#include "RfxLan.h"


namespace picConfigurations
{
   CRfxLan::CRfxLan()
   {
   }

   CRfxLan::~CRfxLan()
   {
   }

   const std::string& CRfxLan::device() const
   {
      static const std::string DeviceName("PIC24FJ256GB106");
      return DeviceName;
   }

   IPicConfiguration::EDeviceType CRfxLan::deviceType() const
   {
      return kPic24Fj;
   }

   unsigned long CRfxLan::programAddressStart() const
   {
      return 0x004000;
   }

   unsigned long CRfxLan::programAddressEnd() const
   {
      return 0x02A7FF;
   }

   unsigned long CRfxLan::bytesPerAddr() const
   {
      return 2;
   }

   unsigned long CRfxLan::eraseBlockSize() const
   {
      return 2048;
   }

   unsigned long CRfxLan::readBlockSize() const
   {
      return 4;
   }

   unsigned long CRfxLan::writeBlockSize() const
   {
      return 256;
   }
} // namespace picConfigurations


