#pragma once
#include "ICurrentTime.h"

namespace shared
{
   namespace currentTime
   {
      //--------------------------------------------------------------
      /// \brief	   The local time implementation of current time
      //--------------------------------------------------------------
      class Local : public ICurrentTime
      {
      public:
         virtual ~Local() = default;

         // ICurrentTime Implementation
         boost::posix_time::ptime now() const override;
         // [END] ICurrentTime Implementation
      };
   }
} // namespace shared::currentTime


