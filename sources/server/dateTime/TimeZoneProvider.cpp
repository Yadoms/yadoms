#include "stdafx.h"
#include "TimeZoneProvider.h"
#include "TimeZoneProviderFromId.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

namespace dateTime
{
   CTimeZoneProvider::CTimeZoneProvider(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                                        boost::shared_ptr<CTimeZoneDatabase> timezoneDatabase,
                                        const std::string& fallbackTimezoneId):
      m_configurationManager(configurationManager),
      m_timezoneDatabase(timezoneDatabase),
      m_fallbackTimezoneId(fallbackTimezoneId)
   {
   }

   CTimeZoneProvider::~CTimeZoneProvider()
   {
   }

   boost::shared_ptr<boost::local_time::posix_time_zone::base_type> CTimeZoneProvider::get() const
   {
      std::string timezoneId;
      try
      {
         const auto location = m_configurationManager->getLocation();
         timezoneId = location->get<std::string>("timezone");

         if (timezoneId == m_lastTimezoneId)
            return m_lastTimezone;
      }
      catch (std::exception&)
      {
         // Not found in database
      }

      if (timezoneId.empty())
      {
         YADOMS_LOG(information) << "Timezone was not found in database, fallback to " << m_fallbackTimezoneId;
         timezoneId = m_fallbackTimezoneId;
      }

      m_lastTimezoneId = timezoneId;

      m_lastTimezone = CTimeZoneProviderFromId(timezoneId,
                                               m_timezoneDatabase).get();

      return m_lastTimezone;
   }
} // namespace dateTime
