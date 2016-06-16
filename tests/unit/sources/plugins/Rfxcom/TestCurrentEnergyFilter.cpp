#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/rfxcomMessages/CurrentEnergyFilter.h"
#include "../../../../sources/shared/shared/currentTime/Provider.h"
#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.hpp"

BOOST_AUTO_TEST_SUITE(TestCurrentEnergyFilter)

BOOST_AUTO_TEST_CASE(DeviceSeen5Times)
{
   CCurrentEnergyFilter filter;
   const std::string newDeviceName("device");
   static auto timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), true);
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
}

BOOST_AUTO_TEST_CASE(DeviceSeen3TimesEachHours)
{
   CCurrentEnergyFilter filter;
   const std::string newDeviceName("device");
   static auto timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProvider->sleep(boost::posix_time::hours(1));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProvider->sleep(boost::posix_time::hours(1));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), true);
}

BOOST_AUTO_TEST_CASE(TimeOver)
{
   CCurrentEnergyFilter filter;
   const std::string newDeviceName("device");
   static auto timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProvider->sleep(boost::posix_time::hours(7));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProvider->sleep(boost::posix_time::hours(7));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProvider->sleep(boost::posix_time::hours(2));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), true);
}

BOOST_AUTO_TEST_SUITE_END()