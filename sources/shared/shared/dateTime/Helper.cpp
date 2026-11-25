#include "stdafx.h"
#include "Helper.h"

namespace shared
{
   namespace dateTime
   {
      boost::posix_time::ptime CHelper::nextMinuteOf(const boost::posix_time::ptime& refTime)
      {
         const boost::posix_time::time_duration nextMinuteOfDay = boost::posix_time::minutes(refTime.time_of_day().total_seconds() / 60);

         return {
            refTime.date(),
            nextMinuteOfDay + boost::posix_time::minutes(1)
         };
      }
   }
} //namespace shared::dateTime
