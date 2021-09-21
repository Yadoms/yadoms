#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

class CProfile_A5_12_Common
{
public:
   enum class E_A5_12_DataType
   {
      kCumulativeValue = 0,
      kCurrentValue = 1
   };

   enum class E_A5_12_Divisor
   {
      kNoDiv = 0,
      kDivBy10 = 1,
      kDivBy100 = 2,
      kDivBy1000 = 3,
   };

   static int applyDivisorInInt(int meterReading,
                                E_A5_12_Divisor divisor,
                                unsigned int multiplier = 1);
   static double applyDivisorInDouble(int meterReading,
                                      E_A5_12_Divisor divisor);

   template <typename T>
   static T clamp(T value, T min, T max);
};

template <typename T>
T CProfile_A5_12_Common::clamp(T value, T min, T max)
{
   if (min > value)
      return min;
   if (max < value)
      return max;
   return value;
}
