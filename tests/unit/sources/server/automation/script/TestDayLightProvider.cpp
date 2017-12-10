#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "automation/script/DayLightProvider.h"
#include "shared/currentTime/Provider.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"
#include "../../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

class CFakeLocation : public shared::ILocation
{
public:
   CFakeLocation(double latitude, double longitude, double altitude = 0.0)
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

BOOST_AUTO_TEST_SUITE(TestDayLightProvider)

   BOOST_AUTO_TEST_CASE(TestDayLightProvider)
   {
      auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-06 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(boost::make_shared<CFakeLocation>(45.05, 5.05));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "08:03");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:58");
   }

   BOOST_AUTO_TEST_CASE(TestDayLightProviderFixBadRoundJVInstallation)
   {
      auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-06 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(boost::make_shared<CFakeLocation>(44.764499999999998, 4.9176000000000002));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "08:02");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:59");
   }

   BOOST_AUTO_TEST_CASE(TestDayLightProviderFixBadRoundJMBInstallation)
   {
      auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>("2017-12-03 00:01:00.000");
      shared::currentTime::Provider().setProvider(timeProviderMock);

      automation::script::CDayLightProvider dlp(boost::make_shared<CFakeLocation>(45.739257, 4.227351));

      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunriseTime()), "08:05");
      BOOST_CHECK_EQUAL(automation::script::CDayLightProvider::formatSunEventTime(dlp.sunsetTime()), "16:59");
   }

BOOST_AUTO_TEST_SUITE_END()
