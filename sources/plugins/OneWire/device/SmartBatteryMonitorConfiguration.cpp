#include "stdafx.h"
#include "SmartBatteryMonitorConfiguration.h"

namespace device
{
   CSmartBatteryMonitorConfiguration::CSmartBatteryMonitorConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration)
   {
      static const shared::CDataContainer::EnumValuesNames EVadSensorTypeNames = boost::assign::map_list_of
         ("none", kNone)
         ("humidity", kHumidity)
         ("HIH3600-humidity", kHIH3600)
         ("HIH4000-humidity", kHIH4000)
         ("HTM1735-humidity", kHTM1735)
         ("S3-R1-A-illuminance", kS3_R1_A)
         ("B1-R1-A-pressure", kB1_R1_A)
         ("raw", kRaw);

      m_vadSensor = configuration->getEnumValue<EVadSensorType>("vadSensor", EVadSensorTypeNames);

      m_visInput = configuration->get<bool>("visSensor");
   }

   CSmartBatteryMonitorConfiguration::EVadSensorType CSmartBatteryMonitorConfiguration::vadSensor() const
   {
      return m_vadSensor;
   }

   bool CSmartBatteryMonitorConfiguration::visInput() const
   {
      return m_visInput;
   }
} // namespace device
