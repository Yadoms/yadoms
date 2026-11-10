#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/dateTime/Helper.h"

// Includes needed to compile the test
#include "../../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

BOOST_AUTO_TEST_SUITE(TestSharedDatetimeHelper)


   BOOST_AUTO_TEST_CASE(NextMinute)
   {
      BOOST_CHECK_EQUAL(shared::dateTime::CHelper::nextMinuteOf(useTimeMock("2022-03-28 23:15:35.000")->now()),
                        boost::posix_time::time_from_string("2022-03-28 23:16:00.000"));

      BOOST_CHECK_EQUAL(shared::dateTime::CHelper::nextMinuteOf(useTimeMock("2022-03-28 23:15:00.000")->now()),
                        boost::posix_time::time_from_string("2022-03-28 23:16:00.000"));

      BOOST_CHECK_EQUAL(shared::dateTime::CHelper::nextMinuteOf(useTimeMock("2022-03-28 23:59:35.000")->now()),
                        boost::posix_time::time_from_string("2022-03-29 00:00:00.000"));

      BOOST_CHECK_EQUAL(shared::dateTime::CHelper::nextMinuteOf(useTimeMock("2022-03-28 23:59:00.000")->now()),
                        boost::posix_time::time_from_string("2022-03-29 00:00:00.000"));
   }


BOOST_AUTO_TEST_SUITE_END()
