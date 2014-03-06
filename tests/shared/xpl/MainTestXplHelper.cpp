// Includes needed to compile tested classes
#include <boost/asio.hpp>
#include <boost/assign.hpp>
#include "../../../sources/shared/shared/exception/BadConversion.hpp"
#include "../../../sources/shared/shared/xpl/XplHelper.h"
#include "../../../sources/shared/shared/xpl/XplException.h"

#define BOOST_TEST_MODULE MainTestXplHelper

#include <boost/test/unit_test.hpp>


//--------------------------------------------------------------
/// \brief	    Test matchRules/checkRules with type kVendorId
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_rulesVendorId)
{
   /* alphanumerical characters, lower case, 1 to 8 characters */

   static const std::list<std::string> okStrings = boost::assign::list_of
      ("a")
      ("5")
      ("abcdefgh")
      ("12345678")
      ("abcd1234")
      ("abcd")
      ("123456");

   static const std::list<std::string> errStrings = boost::assign::list_of
      ("abcdefghi")
      ("123456789")
      ("aBcd1234")
      ("abcd.123")
      ("abcd123Z")
      ("abcd;124")
      ("a-bc-d1")
      ("-ab-c34-")
      ("-")
      (".")
      ("");

   for (std::list<std::string>::const_iterator it = okStrings.begin() ; it != okStrings.end() ; ++it)
   {
      BOOST_CHECK(shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kVendorId, *it));
      BOOST_REQUIRE_NO_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kVendorId, *it));
   }

   for (std::list<std::string>::const_iterator it = errStrings.begin() ; it != errStrings.end() ; ++it)
   {
      BOOST_CHECK(!shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kVendorId, *it));
      BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kVendorId, *it), shared::xpl::CXplException);
   }
}

//--------------------------------------------------------------
/// \brief	    Test matchRules/checkRules with type kDeviceId
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_rulesDeviceId)
{
   /* alphanumerical characters, lower case, 1 to 8 characters */

   static const std::list<std::string> okStrings = boost::assign::list_of
      ("a")
      ("5")
      ("abcdefgh")
      ("12345678")
      ("abcd1234")
      ("abcd")
      ("123456");

   static const std::list<std::string> errStrings = boost::assign::list_of
      ("abcdefghi")
      ("123456789")
      ("aBcd1234")
      ("abcd.123")
      ("abcd123Z")
      ("abcd;124")
      ("a-bc-d1")
      ("-ab-c34-")
      ("-")
      (".")
      ("");

   for (std::list<std::string>::const_iterator it = okStrings.begin() ; it != okStrings.end() ; ++it)
   {
      BOOST_CHECK(shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kDeviceId, *it));
      BOOST_REQUIRE_NO_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kDeviceId, *it));
   }

   for (std::list<std::string>::const_iterator it = errStrings.begin() ; it != errStrings.end() ; ++it)
   {
      BOOST_CHECK(!shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kDeviceId, *it));
      BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kDeviceId, *it), shared::xpl::CXplException);
   }
}

//--------------------------------------------------------------
/// \brief	    Test matchRules/checkRules with type kInstanceId
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_rulesInstanceId)
{
   /* alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */

   static const std::list<std::string> okStrings = boost::assign::list_of
      ("a")
      ("5")
      ("abcdefghijklmnop")
      ("1234567890123456")
      ("abcd1234")
      ("abcd")
      ("123456")
      ("a-bc-d1")
      ("-ab-c34-")
      ("-");

   static const std::list<std::string> errStrings = boost::assign::list_of
      ("abcdefghijklmnopq")
      ("12345678901234567")
      ("aBcd1234")
      ("abcd.123")
      ("abcd123Z")
      ("abcd;124")
      (".")
      ("");

   for (std::list<std::string>::const_iterator it = okStrings.begin() ; it != okStrings.end() ; ++it)
   {
      BOOST_CHECK(shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kInstanceId, *it));
      BOOST_REQUIRE_NO_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kInstanceId, *it));
   }

   for (std::list<std::string>::const_iterator it = errStrings.begin() ; it != errStrings.end() ; ++it)
   {
      BOOST_CHECK(!shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kInstanceId, *it));
      BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kInstanceId, *it), shared::xpl::CXplException);
   }
}

//--------------------------------------------------------------
/// \brief	    Test matchRules/checkRules with type kTypeId
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_rulesTypeId)
{
   /* alphanumerical characters, lower case, '-' accepted, 1 to 8 characters */

   static const std::list<std::string> okStrings = boost::assign::list_of
      ("a")
      ("5")
      ("abcdefgh")
      ("12345678")
      ("abcd1234")
      ("abcd")
      ("123456")
      ("a-bc-d1")
      ("-ab-c34-")
      ("-");

   static const std::list<std::string> errStrings = boost::assign::list_of
      ("abcdefghi")
      ("123456789")
      ("aBcd1234")
      ("abcd.123")
      ("abcd123Z")
      ("abcd;124")
      (".")
      ("");

   for (std::list<std::string>::const_iterator it = okStrings.begin() ; it != okStrings.end() ; ++it)
   {
      BOOST_CHECK(shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kTypeId, *it));
      BOOST_REQUIRE_NO_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kTypeId, *it));
   }

   for (std::list<std::string>::const_iterator it = errStrings.begin() ; it != errStrings.end() ; ++it)
   {
      BOOST_CHECK(!shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kTypeId, *it));
      BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kTypeId, *it), shared::xpl::CXplException);
   }
}

//--------------------------------------------------------------
/// \brief	    Test matchRules/checkRules with type kClassId
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_rulesClassId)
{
   /* alphanumerical characters, lower case, '-' accepted, 1 to 8 characters */

   static const std::list<std::string> okStrings = boost::assign::list_of
      ("a")
      ("5")
      ("abcdefgh")
      ("12345678")
      ("abcd1234")
      ("abcd")
      ("123456")
      ("a-bc-d1")
      ("-ab-c34-")
      ("-");

   static const std::list<std::string> errStrings = boost::assign::list_of
      ("abcdefghi")
      ("123456789")
      ("aBcd1234")
      ("abcd.123")
      ("abcd123Z")
      ("abcd;124")
      (".")
      ("");

   for (std::list<std::string>::const_iterator it = okStrings.begin() ; it != okStrings.end() ; ++it)
   {
      BOOST_CHECK(shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kClassId, *it));
      BOOST_REQUIRE_NO_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kClassId, *it));
   }

   for (std::list<std::string>::const_iterator it = errStrings.begin() ; it != errStrings.end() ; ++it)
   {
      BOOST_CHECK(!shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kClassId, *it));
      BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kClassId, *it), shared::xpl::CXplException);
   }
}

//--------------------------------------------------------------
/// \brief	    Test matchRules/checkRules with type kBody
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_rulesBodyId)
{
   /* alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */

   static const std::list<std::string> okStrings = boost::assign::list_of
      ("a")
      ("5")
      ("abcdefghijklmnop")
      ("1234567890123456")
      ("abcd1234")
      ("abcd")
      ("123456")
      ("a-bc-d1")
      ("-ab-c34-")
      ("-");

   static const std::list<std::string> errStrings = boost::assign::list_of
      ("abcdefghijklmnopq")
      ("12345678901234567")
      ("aBcd1234")
      ("abcd.123")
      ("abcd123Z")
      ("abcd;124")
      (".")
      ("");

   for (std::list<std::string>::const_iterator it = okStrings.begin() ; it != okStrings.end() ; ++it)
   {
      BOOST_CHECK(shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kBody, *it));
      BOOST_REQUIRE_NO_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kBody, *it));
   }

   for (std::list<std::string>::const_iterator it = errStrings.begin() ; it != errStrings.end() ; ++it)
   {
      BOOST_CHECK(!shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kBody, *it));
      BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::checkRules(shared::xpl::CXplHelper::kBody, *it), shared::xpl::CXplException);
   }
}



//--------------------------------------------------------------
/// \brief	    Test toInstanceId(const std::string & elementName)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_toInstanceIdString)
{
   // No modification : output string = input string
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd1234efgh5678"), "abcd1234efgh5678");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("a-bc-d12-34"), "a-bc-d12-34");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("-ab-cd1234-"), "-ab-cd1234-");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd"), "abcd");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("123456"), "123456");

   // With modifications : output string != input string
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd.1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd1234efgh56789"), "abcd1234efgh5678");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("ABCD.1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("aBcd1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd1234Z"), "abcd1234z");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId("abcd;1234"), "abcd1234");
   BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::toInstanceId("."), shared::exception::CBadConversion);
   BOOST_REQUIRE_THROW(shared::xpl::CXplHelper::toInstanceId(""), shared::exception::CBadConversion);
}

//--------------------------------------------------------------
/// \brief	    Test toInstanceId(int elementId)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_toInstanceIdInt)
{
   // No modification
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(1234), "1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(0), "0");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(-1234), "-1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(1632154223), "1632154223");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(2147483647), "2147483647");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(-1), "-1");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toInstanceId(-2147483647), "-2147483647");
}



//TODO : more tests to add