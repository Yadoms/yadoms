#include "stdafx.h"
#include "RFXtrx.h"


namespace picConfigurations
{
   CRfXtrx::CRfXtrx()
   {
   }

   CRfXtrx::~CRfXtrx()
   {
   }

   const std::string& CRfXtrx::device() const
   {
      static const std::string DeviceName("PIC24FJ64GA002");
      return DeviceName;
   }

   IPicConfiguration::EDeviceType CRfXtrx::deviceType() const
   {
      return kPic24Fj;
   }

   unsigned long CRfXtrx::programAddressStart() const
   {
      return 0x001800;
   }

   unsigned long CRfXtrx::programAddressEnd() const
   {
      return 0x00A7FF;
   }

   unsigned long CRfXtrx::bytesPerAddr() const
   {
      return 2;
   }

   unsigned long CRfXtrx::eraseBlockSize() const
   {
      return 2048;
   }

   unsigned long CRfXtrx::readBlockSize() const
   {
      return 4;
   }

   unsigned long CRfXtrx::writeBlockSize() const
   {
      return 256;
   }
} // namespace picConfigurations


