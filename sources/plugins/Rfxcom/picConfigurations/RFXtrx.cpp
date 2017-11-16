#include "stdafx.h"
#include "RFXtrx.h"


namespace picConfigurations
{
   CRFXtrx::CRFXtrx()
   {
   }

   CRFXtrx::~CRFXtrx()
   {
   }

   const std::string& CRFXtrx::device() const
   {
      static const std::string deviceName("PIC24FJ64GA002");
      return deviceName;
   }

   unsigned long CRFXtrx::deviceType() const
   {
      return 2;
   }

   unsigned long CRFXtrx::programAddressStart() const
   {
      return 0x001800;
   }

   unsigned long CRFXtrx::programAddressEnd() const
   {
      return 0x00A7FF;
   }

   unsigned long CRFXtrx::bytesPerAddr() const
   {
      return 2;
   }

   unsigned long CRFXtrx::eraseBlockSize() const
   {
      return 2048;
   }

   unsigned long CRFXtrx::readBlockSize() const
   {
      return 4;
   }

   unsigned long CRFXtrx::writeBlockSize() const
   {
      return 256;
   }
} // namespace picConfigurations


