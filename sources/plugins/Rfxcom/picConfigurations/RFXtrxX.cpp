#include "stdafx.h"
#include "RFXtrxX.h"


namespace picConfigurations
{
   CRfXtrxX::CRfXtrxX()
   {
   }

   CRfXtrxX::~CRfXtrxX()
   {
   }

   const std::string& CRfXtrxX::device() const
   {
      static const std::string DeviceName("PIC24FJ128GA202");
      return DeviceName;
   }

   IPicConfiguration::EDeviceType CRfXtrxX::deviceType() const
   {
      return kPic24Fj;
   }

   unsigned long CRfXtrxX::programAddressStart() const
   {
      return 0x001000;
   }

   unsigned long CRfXtrxX::programAddressEnd() const
   {
      return 0x014FFF;
   }

   unsigned long CRfXtrxX::bytesPerAddr() const
   {
      return 2;
   }

   unsigned long CRfXtrxX::eraseBlockSize() const
   {
      return 2048;
   }

   unsigned long CRfXtrxX::readBlockSize() const
   {
      return 4;
   }

   unsigned long CRfXtrxX::writeBlockSize() const
   {
      return 256;
   }
} // namespace picConfigurations


