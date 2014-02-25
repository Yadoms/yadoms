// Includes needed to compile tested classes
#include <boost/asio.hpp>
#include "../../../sources/shared/shared/exception/BadConversion.hpp"
#include "../../../sources/shared/shared/xpl/XplHelper.h"

#define BOOST_TEST_MODULE MainTestXplHelper

#include <boost/test/unit_test.hpp>


bool checkBadConversionException(shared::exception::CBadConversion exception)
{
   return true;
}



//--------------------------------------------------------------
/// \brief	    Test isStructuralElementMatchRules
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_isStructuralElementMatchRules)
{
   const std::string okStrings[]=
   {
      "abcd1234",
      "a-bc-d12-34",
      "-ab-cd1234-",
      "abcd",
      "123456",
      ""
   };

   const std::string errStrings[]=
   {
      "abcd.1234",
      "aBcd1234",
      "abcd1234Z",
      "abcd;1234",
      ".",
      ""
   };

   for (int idx = 0 ; !okStrings[idx].empty() ; idx++)
      BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::isStructuralElementMatchRules(okStrings[idx]), true);

   for (int idx = 0 ; !errStrings[idx].empty() ; idx++)
      BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::isStructuralElementMatchRules(errStrings[idx]), false);
}

//--------------------------------------------------------------
/// \brief	    Test isVendorIdOrDeviceIdMatchRules
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_isVendorIdOrDeviceIdMatchRules)
{
   const std::string okStrings[]=
   {
      "abcd1234",
      "abcd",
      "123456",
      ""
   };

   const std::string errStrings[]=
   {
      "a-bc-d12-34",
      "-ab-cd1234-",
      "abcd.1234",
      "aBcd1234",
      "abcd1234Z",
      "abcd;1234",
      ".",
      ""
   };

   for (int idx = 0 ; !okStrings[idx].empty() ; idx++)
      BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::isVendorIdOrDeviceIdMatchRules(okStrings[idx]), true);

   for (int idx = 0 ; !errStrings[idx].empty() ; idx++)
      BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::isVendorIdOrDeviceIdMatchRules(errStrings[idx]), false);
}


//--------------------------------------------------------------
/// \brief	    Test toStructuralElement(const std::string & elementName)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_toStructuralElementString)
{
   // No modification
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("abcd1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("a-bc-d12-34"), "a-bc-d12-34");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("-ab-cd1234-"), "-ab-cd1234-");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("abcd"), "abcd");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("123456"), "123456");

   // With modifications
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("abcd.1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("ABCD.1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("aBcd1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("abcd1234Z"), "abcd1234z");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement("abcd;1234"), "abcd1234");
   BOOST_REQUIRE_EXCEPTION(shared::xpl::CXplHelper::toStructuralElement("."), shared::exception::CBadConversion, checkBadConversionException);
}

//--------------------------------------------------------------
/// \brief	    Test toStructuralElement(int elementId)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_toStructuralElementInt)
{
   // No modification
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement(1234), "1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement(0), "0");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement(-1234), "-1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement(1632154223), "1632154223");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toStructuralElement(-1), "-1");
}

//--------------------------------------------------------------
/// \brief	    Test toVendorIdOrDeviceId(const std::string & id)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(XplHelper_toVendorIdOrDeviceId)
{
   // No modification
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("abcd1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("abcd"), "abcd");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("123456"), "123456");

   // With modifications
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("a-bc-d12-34"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("-ab-cd1234-"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("abcd.1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("ABCD.1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("aBcd1234"), "abcd1234");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("abcd1234Z"), "abcd1234z");
   BOOST_CHECK_EQUAL(shared::xpl::CXplHelper::toVendorIdOrDeviceId("abcd;1234"), "abcd1234");
   BOOST_REQUIRE_EXCEPTION(shared::xpl::CXplHelper::toVendorIdOrDeviceId("."), shared::exception::CBadConversion, checkBadConversionException);
   BOOST_REQUIRE_EXCEPTION(shared::xpl::CXplHelper::toVendorIdOrDeviceId("-"), shared::exception::CBadConversion, checkBadConversionException);
}

//TODO : more tests to add