#include "stdafx.h"
#include "TimeZoneProvider.h"
#include "TimeZoneProviderFromId.h"
#include "shared/Log.h"
#include "shared/exception/EmptyResult.hpp"

namespace dateTime
{
   CTimeZoneProvider::CTimeZoneProvider(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                                        boost::shared_ptr<CTimeZoneDatabase> timezoneDatabase,
                                        const std::string& fallbackTimezoneId)
   {
      std::string timezoneId;
      try
      {
         timezoneId = configurationManager->getConfiguration("system", "timezone")->Value();
      }
      catch (shared::exception::CEmptyResult&)
      {
         // Not found in database
      }

      if (timezoneId.empty())
      {
         YADOMS_LOG(information) << "Timezone was not found in database, fallback to " << fallbackTimezoneId;
         timezoneId = fallbackTimezoneId;
      }

      m_timeZone = CTimeZoneProviderFromId(timezoneId,
                                           timezoneDatabase).get();
   }

   CTimeZoneProvider::~CTimeZoneProvider()
   {
   }

   boost::shared_ptr<boost::local_time::posix_time_zone::base_type> CTimeZoneProvider::get() const
   {
      return m_timeZone;
   }
} // namespace dateTime
