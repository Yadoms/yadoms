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

      BOOST_CHECK_NO_THROW(shared::versioning::CVersion(1, 2, 3));
      BOOST_CHECK_NO_THROW(shared::versioning::CVersion(1, 2, 3, "beta.2"));
      BOOST_CHECK_NO_THROW(shared::versioning::CVersion(1, 2, 3, "beta.2", "metaBuild000042"));
      BOOST_CHECK_NO_THROW(shared::versioning::CVersion(1, 2, 3, "", "metaBuild000042"));

      BOOST_CHECK_NO_THROW(shared::versioning::CVersion t2 = shared::versioning::CVersion("1.0.0-beta.2+build0001"));
   }

   BOOST_AUTO_TEST_CASE(extract)
   {
      shared::versioning::CVersion t("2.4.6-beta.2+build0001");
      BOOST_CHECK_EQUAL(t.major(), 2);
      BOOST_CHECK_EQUAL(t.minor(), 4);
      BOOST_CHECK_EQUAL(t.patch(), 6);
      BOOST_CHECK_EQUAL(t.prerelease(), "beta.2");
      BOOST_CHECK_EQUAL(t.buildMetadata(), "build0001");

      shared::versioning::CVersion t2("1.2.3");
      BOOST_CHECK_EQUAL(t2.major(), 1);
      BOOST_CHECK_EQUAL(t2.minor(), 2);
      BOOST_CHECK_EQUAL(t2.patch(), 3);
      BOOST_CHECK_EQUAL(t2.prerelease(), "");
      BOOST_CHECK_EQUAL(t2.buildMetadata(), "");

      shared::versioning::CVersion t3("1.2.3-beta.42");
      BOOST_CHECK_EQUAL(t3.major(), 1);
      BOOST_CHECK_EQUAL(t3.minor(), 2);
      BOOST_CHECK_EQUAL(t3.patch(), 3);
      BOOST_CHECK_EQUAL(t3.prerelease(), "beta.42");
      BOOST_CHECK_EQUAL(t3.buildMetadata(), "");

      shared::versioning::CVersion t4("1.2.3+build0001");
      BOOST_CHECK_EQUAL(t4.major(), 1);
      BOOST_CHECK_EQUAL(t4.minor(), 2);
      BOOST_CHECK_EQUAL(t4.patch(), 3);
      BOOST_CHECK_EQUAL(t4.prerelease(), "");
      BOOST_CHECK_EQUAL(t4.buildMetadata(), "build0001");
   }


   BOOST_AUTO_TEST_CASE(toString)
   {
      BOOST_CHECK_EQUAL(shared::versioning::CVersion().toString(), "0.0.0");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0").toString(), "1.0.0");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0-beta").toString(), "1.0.0-beta");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0-beta.1").toString(), "1.0.0-beta.1");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion("1.0.0-beta.2").toString(), "1.0.0-beta.2");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion("2.0.0-beta.2+build0001").toString(), "2.0.0-beta.2+build0001");

      //use of copy constructor
      shared::versioning::CVersion t("1.0.0-beta.2+build0001");
      shared::versioning::CVersion t2 = t;
      BOOST_CHECK_EQUAL(t.toString(), t2.toString());

      BOOST_CHECK_EQUAL(shared::versioning::CVersion(1, 2, 3).toString(), "1.2.3");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion(1, 2, 3, "alpha").toString(), "1.2.3-alpha");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion(1, 2, 3, "alpha.1").toString(), "1.2.3-alpha.1");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion(1, 2, 3, "alpha.1", "0123456789").toString(), "1.2.3-alpha.1+0123456789");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion(1, 2, 3, "alpha", "0123456789").toString(), "1.2.3-alpha+0123456789");
      BOOST_CHECK_EQUAL(shared::versioning::CVersion(1, 2, 3, "", "0123456789").toString(), "1.2.3+0123456789");
   }


   BOOST_AUTO_TEST_CASE(leftShiftOperator)
   {
      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion();
         BOOST_CHECK_EQUAL(oss.str(), "0.0.0");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion("1.0.0");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion("1.0.0-beta");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0-beta");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion("1.0.0-beta.1");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0-beta.1");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion("1.0.0-beta.2");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0-beta.2");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion("2.0.0-beta.2+build0001");
         BOOST_CHECK_EQUAL(oss.str(), "2.0.0-beta.2+build0001");
      }


      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion(1, 2, 3);
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion(1, 2, 3, "alpha");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion(1, 2, 3, "alpha.1");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha.1");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion(1, 2, 3, "alpha.1", "0123456789");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha.1+0123456789");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion(1, 2, 3, "alpha", "0123456789");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha+0123456789");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CVersion(1, 2, 3, "", "0123456789");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3+0123456789");
      }
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
