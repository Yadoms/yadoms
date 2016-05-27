#pragma once
#include "ISingleReadOnlyData.h"

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Illumination (lux) io accessor interface
   //--------------------------------------------------------------
   class IIllumination : public ISingleReadOnlyData<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IIllumination()
      {
      }
   };

} // namespace ioInterfaces