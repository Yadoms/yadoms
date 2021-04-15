#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/plugin/yPluginApi/historization/Counter.h"

BOOST_AUTO_TEST_SUITE(TestCounter)


   //--------------------------------------------------------------
   /// \brief	    Test generate Nb bits
   /// \result         No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(FormatValue)
   {
      shared::plugin::yPluginApi::historization::CCounter data("name");
      data.set(123);
      BOOST_CHECK_EQUAL(data.formatValue(), "123");
   }

BOOST_AUTO_TEST_SUITE_END()
