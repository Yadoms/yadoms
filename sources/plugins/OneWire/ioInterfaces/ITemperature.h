#pragma once
#include "ISingleReadOnlyData.h"

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Temperature (°C) io accessor interface
   //--------------------------------------------------------------
   class ITemperature : public ISingleReadOnlyData<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ITemperature()
      {
      }
   };

} // namespace ioInterfaces