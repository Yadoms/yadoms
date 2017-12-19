#pragma once
#include <boost/date_time/local_time/posix_time_zone.hpp>

namespace dateTime
{
   //-----------------------------------------------------
   ///\brief The timezone provider interface
   //-----------------------------------------------------
   class ITimeZoneProvider
   {
   public:
      virtual ~ITimeZoneProvider()
      {
      }

      virtual boost::shared_ptr<boost::local_time::posix_time_zone::base_type> get() const = 0;
   };
} // namespace dateTime
