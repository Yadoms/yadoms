#include "stdafx.h"
#include "TimeZoneProviderFromId.h"
#include <boost/date_time/local_time/tz_database.hpp>
#include "TimeZoneDatabase.h"

namespace dateTime
{
   CTimeZoneProviderFromId::CTimeZoneProviderFromId(const std::string& timezoneId,
                                                    const boost::shared_ptr<CTimeZoneDatabase> timezoneDatabase)
      : m_timezone(timezoneDatabase->fromId(timezoneId))
   {
   }

   CTimeZoneProviderFromId::~CTimeZoneProviderFromId()
   {
   }

   boost::shared_ptr<boost::local_time::posix_time_zone::base_type> CTimeZoneProviderFromId::get() const
   {
      return m_timezone;
   }
} // namespace dateTime
