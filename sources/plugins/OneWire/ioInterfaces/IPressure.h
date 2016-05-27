#pragma once
#include "ISingleReadOnlyData.h"

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Pressure (hPa) io accessor interface
   //--------------------------------------------------------------
   class IPressure : public ISingleReadOnlyData<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IPressure()
      {
      }
   };

} // namespace ioInterfaces