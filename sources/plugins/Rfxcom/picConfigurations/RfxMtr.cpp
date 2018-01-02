#include "stdafx.h"
#include "RfxMtr.h"


namespace picConfigurations
{
   CRfxMtr::CRfxMtr()
   {
   }

   CRfxMtr::~CRfxMtr()
   {
   }

   const std::string& CRfxMtr::device() const
   {
      static const std::string DeviceName("PIC24FJ64GB004");
      return DeviceName;
   }

   IPicConfiguration::EDeviceType CRfxMtr::deviceType() const
   {
      return kPic24Fj;
   }

   unsigned long CRfxMtr::programAddressStart() const
   {
      return 0x002400;
   }

   unsigned long CRfxMtr::programAddressEnd() const
   {
      return 0x00A7FF;
   }

   unsigned long CRfxMtr::bytesPerAddr() const
   {
      return 2;
   }

   unsigned long CRfxMtr::eraseBlockSize() const
   {
      return 2048;
   }

   unsigned long CRfxMtr::readBlockSize() const
   {
      return 4;
   }

   unsigned long CRfxMtr::writeBlockSize() const
   {
      return 256;
   }
} // namespace picConfigurations


