#pragma once
#include "ISingleReadOnlyData.h"

namespace ioInterfaces {

   //--------------------------------------------------------------
   /// \brief	Voltage (V) io accessor interface
   //--------------------------------------------------------------
   class IVoltage : public ISingleReadOnlyData<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IVoltage() {}
   };

} // namespace ioInterfaces