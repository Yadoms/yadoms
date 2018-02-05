#pragma once
#include <boost/date_time/local_time/posix_time_zone.hpp>
#include <boost/date_time/local_time/tz_database.hpp>

namespace dateTime
{
   //-----------------------------------------------------
   ///\brief The timezone database
   ///\description It's just a copy/paste of \boost\libs\date_time\data\date_time_zonespec.csv file
   //-----------------------------------------------------
   class CTimeZoneDatabase
   {
   public:
      CTimeZoneDatabase();
      virtual ~CTimeZoneDatabase();
      boost::shared_ptr<boost::local_time::posix_time_zone::base_type> fromId(const std::string& timezoneId) const;
      std::vector<std::string> allIds() const;

   private:
      static const std::string DateTimeZonespecCsvImport;
      boost::local_time::tz_database m_tzDb;
   };
} // namespace dateTime
