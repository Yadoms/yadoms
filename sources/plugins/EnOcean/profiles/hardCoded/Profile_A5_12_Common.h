#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

class CProfile_A5_12_Common
{
public:
   enum E_A5_12_DataType
   {
      kCumulativeValue = 0,
      kCurrentValue = 1
   };

   enum E_A5_12_Divisor
   {
      kNoDiv = 0,
      kDivBy10 = 1,
      kDivBy100 = 2,
      kDivBy1000 = 3,
   };

   static int applyDivisorInInt(const int meterReading,
                                const E_A5_12_Divisor divisor);
   static double applyDivisorInDouble(const int meterReading,
                                      const E_A5_12_Divisor divisor);
};
