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
      CSmartBatteryMonitorConfiguration(const shared::CDataContainer& configuration);
      virtual ~CSmartBatteryMonitorConfiguration();

   protected:
      bool historizeHumidity() const;
      bool historizeLight() const;

   private:
      bool m_historizeHumidity;
      bool m_historizeLight;
   };

} // namespace device
