#pragma once

namespace nitramMessages
{
   typedef enum
   {
      BatteryCapacityAndVoltage = 32,
      Runtime = 33,
      SummaryStatus = 34,
      Ack = 0x40,
      SetupShutdownDelay = 198,
      SetupStartupDelay = 199
   } ENitramMessageID;
} // namespace nitramMessages