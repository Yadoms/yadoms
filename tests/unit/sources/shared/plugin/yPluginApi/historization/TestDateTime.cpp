#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/plugin/yPluginApi/historization/DateTime.h"

BOOST_AUTO_TEST_SUITE(TestDateTime)


   //--------------------------------------------------------------
   /// \brief	    Test generate Nb bits
   /// \result         No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(FormatValue)
   {
      shared::plugin::yPluginApi::historization::CDateTime iso("name");
      iso.set(boost::posix_time::from_time_t(1596632400));
      BOOST_CHECK_EQUAL(iso.formatValue(), "20200805T130000");
      iso.setFromUnixTime(1596632400);
      BOOST_CHECK_EQUAL(iso.formatValue(), "20200805T130000");

      shared::plugin::yPluginApi::historization::CDateTime unix("name");
      unix.set(boost::posix_time::time_from_string("2020-08-05 13:00:00"));
      BOOST_CHECK_EQUAL(unix.formatValue(), "20200805T130000");
      unix.setFromIso("2020-08-05 13:00:00");
      BOOST_CHECK_EQUAL(unix.formatValue(), "20200805T130000");
   }

BOOST_AUTO_TEST_SUITE_END()
