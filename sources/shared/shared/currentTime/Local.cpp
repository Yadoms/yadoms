#include "stdafx.h"
#include "Local.h"

namespace shared
{
   namespace currentTime
   {
      boost::posix_time::ptime Local::now() const
      {
         return boost::posix_time::microsec_clock::local_time();
      }
   }
} // namespace shared::currentTime


