#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "automation/script/DayLightProvider.h"
#include "shared/currentTime/Provider.h"
#include "dateTime/ITimeZoneProvider.h"

// Includes needed to compile the test
#include "../../../mock/shared/currentTime/DefaultCurrentTimeMock.h"
#include <boost/date_time/local_time/tz_database.hpp>

class CFakeLocation : public shared::ILocation
{
public:
   CFakeLocation(const double latitude, const double longitude, const double altitude = 0.0)
      : m_latitude(latitude), m_longitude(longitude), m_altitude(altitude)
   {
   }

   virtual ~CFakeLocation()
   {
   }

   double latitude() const override
   {
      return m_latitude;
   }

   double longitude() const override
   {
      return m_longitude;
   }

   double altitude() const override
   {
      return m_altitude;
   }

private:
   double m_latitude;
   double m_longitude;
   double m_altitude;
};

class CFakeTimezoneProvider : public dateTime::ITimeZoneProvider
{
public:
   explicit CFakeTimezoneProvider(const std::string timezoneId)
   {
      boost::local_time::tz_database tzDb;

      static const std::string DateTimeZonespecCsvImport =
         "\"Europe/Paris\",\"CET\",\"CET\",\"CEST\",\"CEST\",\"+01:00:00\",\"+01:00:00\",\"-1;0;3\",\"+02:00:00\",\"-1;0;10\",\"+03:00:00\"" "\n"
         "\"America/New_York\",\"EST\",\"Eastern Standard Time\",\"EDT\",\"Eastern Daylight Time\",\"-05:00:00\",\"+01:00:00\",\"2;0;3\",\"+02:00:00\",\"1;0;11\",\"+02:00:00\"";

      std::istringstream dateTimeZonespecCsvImportStream(DateTimeZonespecCsvImport);
      tzDb.load_from_stream(dateTimeZonespecCsvImportStream);
      m_timeZone = tzDb.time_zone_from_region(timezoneId);

      if (!m_timeZone)
         throw std::invalid_argument("Unknown timezone ID");
   }

   virtual ~CFakeTimezoneProvider()
   {
   }

   boost::shared_ptr<boost::local_time::posix_time_zone::base_type> get() const override
   {
      return m_timeZone;
   }

private:
   boost::shared_ptr<boost::local_time::posix_time_zone::base_type> m_timeZone;
};


BOOST_AUTO_TEST_SUITE(TestDayLightProvider)

   BOOST_AUTO_TEST_CASE(TestDayLightProvider)
   {
      const auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-06 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(boost::make_shared<CFakeLocation>(45.05, 5.05),
         boost::make_shared<CFakeTimezoneProvider>("Europe/Paris"));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "08:03");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:58");
   }

   BOOST_AUTO_TEST_CASE(TestDayLightProviderFixBadRoundJVInstallation)
   {
      const auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-06 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(
         boost::make_shared<CFakeLocation>(44.764499999999998, 4.9176000000000002),
         boost::make_shared<CFakeTimezoneProvider>("Europe/Paris"));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "08:02");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:59");
   }

   BOOST_AUTO_TEST_CASE(TestDayLightProviderFixBadRoundJMBInstallation)
   {
      const auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-03 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(boost::make_shared<CFakeLocation>(45.739257, 4.227351),
         boost::make_shared<CFakeTimezoneProvider>("Europe/Paris"));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "08:05");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:59");
   }

   BOOST_AUTO_TEST_CASE(TestDayLightProviderFixBadTimezoneNewYork)
   {
      const auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-11 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(boost::make_shared<CFakeLocation>(40.612542, -74.153860),
         boost::make_shared<CFakeTimezoneProvider>("America/New_York"));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "07:10");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:29");
   }

BOOST_AUTO_TEST_SUITE_END()
