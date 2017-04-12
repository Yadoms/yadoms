#pragma once
#include "ICurrentTime.h"

namespace shared
{
   namespace currentTime
   {
      //--------------------------------------------------------------
      /// \brief	   The local time implementation of current time
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT Local : public ICurrentTime
      {
      public:
         virtual ~Local();

         // ICurrentTime Implementation
         boost::posix_time::ptime now() const override;
         // [END] ICurrentTime Implementation
      };
   }
} // namespace shared::currentTime


