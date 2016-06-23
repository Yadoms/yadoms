#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/rfxcomMessages/CurrentEnergyFilter.h"

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

BOOST_AUTO_TEST_SUITE(TestCurrentEnergyFilter)

BOOST_AUTO_TEST_CASE(DeviceSeen5Times)
{
   CCurrentEnergyFilter filter;
   const std::string newDeviceName("device");

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

   auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>();
   shared::currentTime::Provider().setProvider(timeProviderMock);

   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProviderMock->sleep(boost::posix_time::hours(1));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProviderMock->sleep(boost::posix_time::hours(1));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), true);
}

BOOST_AUTO_TEST_CASE(TimeOver)
{
   CCurrentEnergyFilter filter;
   const std::string newDeviceName("device");

   auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>();
   shared::currentTime::Provider().setProvider(timeProviderMock);

   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProviderMock->sleep(boost::posix_time::hours(7));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProviderMock->sleep(boost::posix_time::hours(7));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), false);
   timeProviderMock->sleep(boost::posix_time::hours(2));
   BOOST_CHECK_EQUAL(filter.isValid(newDeviceName), true);
}

BOOST_AUTO_TEST_SUITE_END()