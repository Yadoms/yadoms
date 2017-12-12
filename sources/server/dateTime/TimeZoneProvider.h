#pragma once
#include "ITimeZoneProvider.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "TimeZoneDatabase.h"

namespace dateTime
{
   //-----------------------------------------------------
   ///\brief The timezone provider
   //-----------------------------------------------------
   class CTimeZoneProvider : public ITimeZoneProvider
   {
   public:
      CTimeZoneProvider(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                        boost::shared_ptr<CTimeZoneDatabase> timezoneDatabase,
                        const std::string& fallbackTimezoneId);
      virtual ~CTimeZoneProvider();

      // ITimeZoneProvider Implementation
      boost::shared_ptr<boost::local_time::posix_time_zone::base_type> get() const override;
      // [END] ITimeZoneProvider Implementation

   private:
      boost::shared_ptr<boost::local_time::posix_time_zone::base_type> m_timeZone;
   };
} // namespace dateTime
