#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/versioning/SemVer.h"
#include <shared/Log.h>

BOOST_AUTO_TEST_SUITE(TestSemVer)


   BOOST_AUTO_TEST_CASE(Constructor)
   {
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t);
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t("1.0.0"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t("1.0.0-beta"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t("1.0.0-beta.1"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t("1.0.0-beta.2"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t("1.0.0-beta.2+build0001"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t("1.0.0+build0001"));
      BOOST_CHECK_THROW(shared::versioning::CSemVer t(""), std::exception);
      BOOST_CHECK_THROW(shared::versioning::CSemVer t("1"), std::exception);
      BOOST_CHECK_THROW(shared::versioning::CSemVer t("1.0"), std::exception);
      BOOST_CHECK_THROW(shared::versioning::CSemVer t("1.0.a"), std::exception);
      BOOST_CHECK_THROW(shared::versioning::CSemVer t("1.0.0.1"), std::exception);
      BOOST_CHECK_THROW(shared::versioning::CSemVer t("1.0.0.beta"), std::exception);

      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer(1, 2, 3));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer(1, 2, 3, "beta.2"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer(1, 2, 3, "beta.2", "metaBuild000042"));
      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer(1, 2, 3, "", "metaBuild000042"));

      BOOST_CHECK_NO_THROW(shared::versioning::CSemVer t2 = shared::versioning::CSemVer("1.0.0-beta.2+build0001"));
   }

   BOOST_AUTO_TEST_CASE(extract)
   {
      shared::versioning::CSemVer t("2.4.6-beta.2+build0001");
      BOOST_CHECK_EQUAL(t.getMajor(), 2);
      BOOST_CHECK_EQUAL(t.getMinor(), 4);
      BOOST_CHECK_EQUAL(t.getPatch(), 6);
      BOOST_CHECK_EQUAL(t.getPrerelease(), "beta.2");
      BOOST_CHECK_EQUAL(t.getBuildMetadata(), "build0001");

      shared::versioning::CSemVer t2("1.2.3");
      BOOST_CHECK_EQUAL(t2.getMajor(), 1);
      BOOST_CHECK_EQUAL(t2.getMinor(), 2);
      BOOST_CHECK_EQUAL(t2.getPatch(), 3);
      BOOST_CHECK_EQUAL(t2.getPrerelease(), "");
      BOOST_CHECK_EQUAL(t2.getBuildMetadata(), "");

      shared::versioning::CSemVer t3("1.2.3-beta.42");
      BOOST_CHECK_EQUAL(t3.getMajor(), 1);
      BOOST_CHECK_EQUAL(t3.getMinor(), 2);
      BOOST_CHECK_EQUAL(t3.getPatch(), 3);
      BOOST_CHECK_EQUAL(t3.getPrerelease(), "beta.42");
      BOOST_CHECK_EQUAL(t3.getBuildMetadata(), "");

      shared::versioning::CSemVer t4("1.2.3+build0001");
      BOOST_CHECK_EQUAL(t4.getMajor(), 1);
      BOOST_CHECK_EQUAL(t4.getMinor(), 2);
      BOOST_CHECK_EQUAL(t4.getPatch(), 3);
      BOOST_CHECK_EQUAL(t4.getPrerelease(), "");
      BOOST_CHECK_EQUAL(t4.getBuildMetadata(), "build0001");
   }


   BOOST_AUTO_TEST_CASE(toString)
   {
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer().toString(), "0.0.0");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer("1.0.0").toString(), "1.0.0");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer("1.0.0-beta").toString(), "1.0.0-beta");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer("1.0.0-beta.1").toString(), "1.0.0-beta.1");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer("1.0.0-beta.2").toString(), "1.0.0-beta.2");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer("2.0.0-beta.2+build0001").toString(), "2.0.0-beta.2+build0001");

      //use of copy constructor
      const shared::versioning::CSemVer t("1.0.0-beta.2+build0001");
      const auto t2 = t;
      BOOST_CHECK_EQUAL(t.toString(), t2.toString());

      BOOST_CHECK_EQUAL(shared::versioning::CSemVer(1, 2, 3).toString(), "1.2.3");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer(1, 2, 3, "alpha").toString(), "1.2.3-alpha");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer(1, 2, 3, "alpha.1").toString(), "1.2.3-alpha.1");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer(1, 2, 3, "alpha.1", "0123456789").toString(), "1.2.3-alpha.1+0123456789");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer(1, 2, 3, "alpha", "0123456789").toString(), "1.2.3-alpha+0123456789");
      BOOST_CHECK_EQUAL(shared::versioning::CSemVer(1, 2, 3, "", "0123456789").toString(), "1.2.3+0123456789");
   }


   BOOST_AUTO_TEST_CASE(leftShiftOperator)
   {
      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer();
         BOOST_CHECK_EQUAL(oss.str(), "0.0.0");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer("1.0.0");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer("1.0.0-beta");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0-beta");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer("1.0.0-beta.1");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0-beta.1");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer("1.0.0-beta.2");
         BOOST_CHECK_EQUAL(oss.str(), "1.0.0-beta.2");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer("2.0.0-beta.2+build0001");
         BOOST_CHECK_EQUAL(oss.str(), "2.0.0-beta.2+build0001");
      }


      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer(1, 2, 3);
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer(1, 2, 3, "alpha");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer(1, 2, 3, "alpha.1");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha.1");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer(1, 2, 3, "alpha.1", "0123456789");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha.1+0123456789");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer(1, 2, 3, "alpha", "0123456789");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3-alpha+0123456789");
      }

      {
         std::ostringstream oss;
         oss << shared::versioning::CSemVer(1, 2, 3, "", "0123456789");
         BOOST_CHECK_EQUAL(oss.str(), "1.2.3+0123456789");
      }
   }

   BOOST_AUTO_TEST_CASE(compare)
   {
      BOOST_CHECK(shared::versioning::CSemVer() == shared::versioning::CSemVer());
      BOOST_CHECK(shared::versioning::CSemVer("1.2.4") > shared::versioning::CSemVer("1.2.3"));
      BOOST_CHECK(shared::versioning::CSemVer("1.3.0") > shared::versioning::CSemVer("1.2.3"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0") > shared::versioning::CSemVer("1.2.3"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0") > shared::versioning::CSemVer("2.0.0-beta"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0-beta.2") > shared::versioning::CSemVer("2.0.0-beta.1"));
      BOOST_CHECK(shared::versioning::CSemVer("1.2.4+build0001") > shared::versioning::CSemVer("1.2.3+build0001"));
      BOOST_CHECK(shared::versioning::CSemVer("1.3.0+build0001") > shared::versioning::CSemVer("1.2.3+build0001"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0+build0001") > shared::versioning::CSemVer("1.2.3+build0001"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0+build0001") > shared::versioning::CSemVer("2.0.0-beta+build0001"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0-beta.2+build0001") > shared::versioning::CSemVer("2.0.0-beta.1+build0001"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0-beta.2+build0001") == shared::versioning::CSemVer("2.0.0-beta.2+build0002"));
      BOOST_CHECK(shared::versioning::CSemVer("2.0.0+build0001") == shared::versioning::CSemVer("2.0.0+build0002"));
   }

   BOOST_AUTO_TEST_SUITE_END()
