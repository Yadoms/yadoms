#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/versioning/Version.h"
#include <shared/Log.h>

BOOST_AUTO_TEST_SUITE(TestVersion)



BOOST_AUTO_TEST_CASE(Constructor)
{
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t);
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t("1.0.0"));
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t("1.0.0-beta"));
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t("1.0.0-beta.1"));
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t("1.0.0-beta.2"));
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t("1.0.0-beta.2+build0001"));
   BOOST_CHECK_NO_THROW(shared::versioning::CVersion t("1.0.0+build0001"));
   BOOST_CHECK_THROW(shared::versioning::CVersion t(""), std::exception);
   BOOST_CHECK_THROW(shared::versioning::CVersion t("1"), std::exception);
   BOOST_CHECK_THROW(shared::versioning::CVersion t("1.0"), std::exception);
   BOOST_CHECK_THROW(shared::versioning::CVersion t("1.0.a"), std::exception);
   BOOST_CHECK_THROW(shared::versioning::CVersion t("1.0.0.1"), std::exception);
   BOOST_CHECK_THROW(shared::versioning::CVersion t("1.0.0.beta"), std::exception);

}

BOOST_AUTO_TEST_CASE(toString)
{
   BOOST_CHECK_EQUAL(shared::versioning::CVersion().toString(), "0.0.0");
   BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0").toString(), "1.0.0");
   BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0-beta").toString(), "1.0.0-beta");
   BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0-beta.1").toString(), "1.0.0-beta.1");
   BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0-beta.2").toString(), "1.0.0-beta.2");
   BOOST_CHECK_EQUAL(shared::versioning::CVersion("2.0.0-beta.2+build0001").toString(), "2.0.0-beta.2+build0001");
}

BOOST_AUTO_TEST_CASE(compare)
{
   BOOST_CHECK(shared::versioning::CVersion() == shared::versioning::CVersion());
   BOOST_CHECK(shared::versioning::CVersion("1.2.4") > shared::versioning::CVersion("1.2.3"));
   BOOST_CHECK(shared::versioning::CVersion("1.3.0") > shared::versioning::CVersion("1.2.3"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0") > shared::versioning::CVersion("1.2.3"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0") > shared::versioning::CVersion("2.0.0-beta"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0-beta.2") > shared::versioning::CVersion("2.0.0-beta.1"));
   BOOST_CHECK(shared::versioning::CVersion("1.2.4+build0001") > shared::versioning::CVersion("1.2.3+build0001"));
   BOOST_CHECK(shared::versioning::CVersion("1.3.0+build0001") > shared::versioning::CVersion("1.2.3+build0001"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0+build0001") > shared::versioning::CVersion("1.2.3+build0001"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0+build0001") > shared::versioning::CVersion("2.0.0-beta+build0001"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0-beta.2+build0001") > shared::versioning::CVersion("2.0.0-beta.1+build0001"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0-beta.2+build0001") == shared::versioning::CVersion("2.0.0-beta.2+build0002"));
   BOOST_CHECK(shared::versioning::CVersion("2.0.0+build0001") == shared::versioning::CVersion("2.0.0+build0002"));
}

BOOST_AUTO_TEST_SUITE_END()
