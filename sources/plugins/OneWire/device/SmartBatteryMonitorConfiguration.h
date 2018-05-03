#pragma once
#include <shared/DataContainer.h>

namespace device
{
   //--------------------------------------------------------------
   /// \brief	Battery monitor device configuration
   //--------------------------------------------------------------
   class CSmartBatteryMonitorConfiguration
   {
   public:
      enum EVadSensorType
      {
         kNone = 0,
         kHumidity,
         kHIH3600,
         kHIH4000,
         kHTM1735,
         kS3_R1_A,
         kB1_R1_A,
         kRaw
      };

   public:
      explicit CSmartBatteryMonitorConfiguration(const shared::CDataContainer& configuration);
      virtual ~CSmartBatteryMonitorConfiguration();

      EVadSensorType vadSensor() const;
      bool visInput() const;

   private:
      EVadSensorType m_vadSensor;
      bool m_visInput;
   };

} // namespace device
