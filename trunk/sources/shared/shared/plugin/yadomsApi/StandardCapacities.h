#pragma once
#include <shared/Export.h>
#include "Capacity.h"


namespace shared { namespace plugin { namespace yadomsApi
{
   //--------------------------------------------------------------
   /// \brief		Standard capacities
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CStandardCapacities //TODO utile ?
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Temperature sensor capacity
      /// \note      Temperature is in degrees Celcius
      //--------------------------------------------------------------
      static const CCapacity& getTemperatureSensorCapacity();
      
      ////--------------------------------------------------------------
      ///// \brief		Battery level capacity
      ///// \note      Temperature is in percent
      ////--------------------------------------------------------------
      static const CCapacity& getBatteryLevelCapacity();

      ////--------------------------------------------------------------
      ///// \brief		Rssi measure capacity
      ///// \note      Rssi value is in percent
      ////--------------------------------------------------------------
      static const CCapacity& getRssiMeasureCapacity();
   };

} } } // namespace shared::plugin::yadomsApi

