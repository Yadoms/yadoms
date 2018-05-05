#include "stdafx.h"
#include "Profile_A5_12_Common.h"


int CProfile_A5_12_Common::applyDivisorInInt(const int meterReading,
                                             const E_A5_12_Divisor divisor)
{
   switch (divisor)
   {
   case kDivBy10:
      return meterReading / 10;
   case kDivBy100:
      return meterReading / 100;
   case kDivBy1000:
      return meterReading / 1000;
   default:
      return meterReading;
   }
}

double CProfile_A5_12_Common::applyDivisorInDouble(const int meterReading,
                                                   const E_A5_12_Divisor divisor)
{
   switch (divisor)
   {
   case kDivBy10:
      return static_cast<double>(meterReading) / 10.0;
   case kDivBy100:
      return static_cast<double>(meterReading) / 100.0;
   case kDivBy1000:
      return static_cast<double>(meterReading) / 1000.0;
   default:
      return static_cast<double>(meterReading);
   }
}
