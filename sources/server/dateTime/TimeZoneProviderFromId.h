#pragma once
#include "ITimeZoneProvider.h"
#include "TimeZoneDatabase.h"

namespace dateTime
{
   //-----------------------------------------------------
   ///\brief The day light helper
   //-----------------------------------------------------
   class CTimeZoneProviderFromId : public ITimeZoneProvider
   {
   public:
      CTimeZoneProviderFromId(const std::string& timezoneId,
                              const boost::shared_ptr<const CTimeZoneDatabase> timezoneDatabase);
      virtual ~CTimeZoneProviderFromId();

      // ITimeZoneProvider Implementation
      boost::shared_ptr<boost::local_time::posix_time_zone::base_type> get() const override;
      // [END] ITimeZoneProvider Implementation

   private:
      boost::shared_ptr<boost::local_time::posix_time_zone::base_type> m_timezone;
   };
} // namespace dateTime
