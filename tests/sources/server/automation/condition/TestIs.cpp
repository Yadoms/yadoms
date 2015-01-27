#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../../sources/server/automation/condition/Is.h"

// Mocks
#include "Mocks.hpp"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestIs)

//--------------------------------------------------------------
/// \brief	    Bool based keyword (ie : Switch)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(BoolBasedKeyword)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"equal\", \"refValue\" : \"1\" }");
   automation::condition::CIs is(configuration, lastAcq);

   BOOST_CHECK_EQUAL(is.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(is.eval(), false);
   is.onKeywordStateChange("1");
   BOOST_CHECK_EQUAL(is.eval(), true);
   is.onKeywordStateChange("0");
   BOOST_CHECK_EQUAL(is.eval(), false);
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, equal operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordEqualOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"equal\", \"refValue\" : \"13\" }");
   automation::condition::CIs is(configuration, lastAcq);

   BOOST_CHECK_EQUAL(is.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(is.eval(), false);
   for (int i = 0; i < 20; ++i)
   {
      is.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(is.eval(), i == 13); // true only if i==13
   }
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, >= operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordGreaterOrEqualOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"greaterOrEqual\", \"refValue\" : \"13\" }");
   automation::condition::CIs is(configuration, lastAcq);

   BOOST_CHECK_EQUAL(is.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(is.eval(), false);
   for (int i = 0; i < 20; ++i)
   {
      is.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(is.eval(), i >= 13);
   }
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, < operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordLowerOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"lower\", \"refValue\" : \"13\" }");
   automation::condition::CIs is(configuration, lastAcq);

   BOOST_CHECK_EQUAL(is.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(is.eval(), true);
   for (int i = 0; i < 20; ++i)
   {
      is.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(is.eval(), i < 13);
   }
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, != operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordDifferentOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"different\", \"refValue\" : \"13\" }");
   automation::condition::CIs is(configuration, lastAcq);

   BOOST_CHECK_EQUAL(is.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(is.eval(), true);
   for (int i = 0; i < 20; ++i)
   {
      is.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(is.eval(), i != 13);
   }
}
             
//--------------------------------------------------------------
/// \brief	    Unsupported operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(UnsupportedOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"unsupportedOperator\", \"refValue\" : \"13\" }");
   BOOST_CHECK_THROW(automation::condition::CIs is(configuration, lastAcq), shared::exception::COutOfRange);
}


BOOST_AUTO_TEST_SUITE_END()