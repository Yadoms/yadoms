#include "stdafx.h"
#include "SmartBatteryMonitorConfiguration.h"

namespace device
{
   CSmartBatteryMonitorConfiguration::CSmartBatteryMonitorConfiguration(const shared::CDataContainer& configuration)
   {
      m_historizeHumidity = configuration.get<bool>("humidity");
      m_historizeLight = configuration.get<bool>("light");
   }

   CSmartBatteryMonitorConfiguration::~CSmartBatteryMonitorConfiguration()
   {
   }

   bool CSmartBatteryMonitorConfiguration::historizeHumidity() const
   {
      return m_historizeHumidity;
   }

   bool CSmartBatteryMonitorConfiguration::historizeLight() const
   {
      return m_historizeLight;
   }
} // namespace device
