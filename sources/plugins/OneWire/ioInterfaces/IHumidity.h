#pragma once
#include "ISingleReadOnlyData.h"

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Humidity (%) io accessor interface
   //--------------------------------------------------------------
   class IHumidity : public ISingleReadOnlyData<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IHumidity()
      {
      }
   };

} // namespace ioInterfaces