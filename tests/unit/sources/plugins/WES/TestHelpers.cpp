#include "stdafx.h"
#include <boost/test/unit_test.hpp>

#include "../../../../sources/plugins/WES/equipments/Helpers.h"
#include "../../../../sources/plugins/WES/equipments/tooLowRevisionException.hpp"

BOOST_AUTO_TEST_SUITE(TestHelpers)

   BOOST_AUTO_TEST_CASE(checkRevision)
   {
	  BOOST_CHECK_THROW(equipments::checkRevision(std::string("V0.70"));, std::exception);
	  BOOST_CHECK_THROW(equipments::checkRevision(std::string("V0.83G")); , CtooLowRevisionException);
	  BOOST_CHECK_NO_THROW(equipments::checkRevision(std::string("V0.83H B03")););
	  BOOST_CHECK_NO_THROW(equipments::checkRevision(std::string("V0.83H B04")););
	  BOOST_CHECK_NO_THROW(equipments::checkRevision(std::string("V0.83H")););
	  BOOST_CHECK_NO_THROW(equipments::checkRevision(std::string("V0.83I")););
   }

BOOST_AUTO_TEST_SUITE_END()
