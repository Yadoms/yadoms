#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/server/tools/tools/web/UriSinglePatternValidator.h"

BOOST_AUTO_TEST_SUITE(TestUriValidator)


BOOST_AUTO_TEST_CASE(SingleUriPart)
{
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid(""), false);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid(" "), false);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("with space "), false);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("withoutspace"), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok"), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok#"), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok."), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok@"), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok-"), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok_"), true);
   BOOST_CHECK_EQUAL(tools::web::CUriSinglePatternValidator::isValid("ok@.#-_"), true);
}


BOOST_AUTO_TEST_SUITE_END()
