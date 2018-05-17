#include "stdafx.h"
#include "Profile_A5_12_Common.h"


int CProfile_A5_12_Common::applyDivisorInInt(int meterReading,
                                             E_A5_12_Divisor divisor,
                                             unsigned int multiplier)
{
   switch (divisor)
   {
   case kDivBy10:
      return meterReading * (multiplier / 10);
   case kDivBy100:
      return meterReading * (multiplier / 100);
   case kDivBy1000:
      return meterReading * (multiplier / 1000);
   default:
      return meterReading;
   }
}

double CProfile_A5_12_Common::applyDivisorInDouble(int meterReading,
                                                   E_A5_12_Divisor divisor)
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
