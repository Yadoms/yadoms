#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../../sources/server/automation/condition/Become.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestBecome)


class CDatabaseAcquisitionRequesterMock : public database::IAcquisitionRequester
{
public:
   virtual boost::shared_ptr<database::entities::CAcquisition> saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime) { return boost::shared_ptr<database::entities::CAcquisition>(); }
   virtual boost::shared_ptr<database::entities::CAcquisition> incrementData(const int keywordId, const std::string & increment, boost::posix_time::ptime & dataTime) { return boost::shared_ptr<database::entities::CAcquisition>(); }
   virtual LastSummaryData saveSummaryData(const int keywordId, boost::posix_time::ptime & dataTime) { return LastSummaryData(); }
   virtual void removeKeywordData(const int keywordId) {}
   virtual boost::shared_ptr<database::entities::CAcquisition> getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time) { return boost::shared_ptr<database::entities::CAcquisition>(); }
   virtual boost::shared_ptr<database::entities::CAcquisition> getKeywordLastData(const int keywordId)
   {
      boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition());
      acq->KeywordId = keywordId;
      acq->Value = m_value;
      return acq;
   }
   virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string> > getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::vector< boost::tuple<boost::posix_time::ptime, std::string> >(); }
   virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> >(); }
   virtual std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> >(); }
   virtual std::string getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::string(); }
   virtual std::string getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::string(); }
   virtual std::string getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) { return std::string(); }
   std::string m_value;
};

//--------------------------------------------------------------
/// \brief	    Bool based keyword (ie : Switch)
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(BoolBasedKeyword)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"equal\", \"refValue\" : \"1\" }");
   automation::condition::CBecome become(configuration, lastAcq);

   BOOST_CHECK_EQUAL(become.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(become.eval(), false);
   become.onKeywordStateChange("1");
   BOOST_CHECK_EQUAL(become.eval(), true);
   BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
   BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
   become.onKeywordStateChange("0");
   BOOST_CHECK_EQUAL(become.eval(), false);
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, equal operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordEqualOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"equal\", \"refValue\" : \"13\" }");
   automation::condition::CBecome become(configuration, lastAcq);

   BOOST_CHECK_EQUAL(become.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(become.eval(), false);
   for (int i = 0; i < 20; ++i)
   {
      become.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(become.eval(), i == 13); // true only if i==13
      BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
      BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
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
   automation::condition::CBecome become(configuration, lastAcq);

   BOOST_CHECK_EQUAL(become.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(become.eval(), false);
   for (int i = 0; i < 20; ++i)
   {
      become.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(become.eval(), i == 13); // true when i becomes >= 13
   }
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, < operator
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordLowerOperator)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "20";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"lower\", \"refValue\" : \"13\" }");
   automation::condition::CBecome become(configuration, lastAcq);

   BOOST_CHECK_EQUAL(become.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(become.eval(), false);
   for (int i = 20; i >= 0; --i)
   {
      become.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(become.eval(), i == 12);
      BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
      BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
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
   automation::condition::CBecome become(configuration, lastAcq);

   BOOST_CHECK_EQUAL(become.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(become.eval(), true);
   BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
   BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
   become.onKeywordStateChange("11");
   BOOST_CHECK_EQUAL(become.eval(), false); // Still false
   BOOST_CHECK_EQUAL(become.eval(), false); // Still false
   become.onKeywordStateChange("13");
   BOOST_CHECK_EQUAL(become.eval(), false); // Still false
   BOOST_CHECK_EQUAL(become.eval(), false); // Still false
   become.onKeywordStateChange("18");
   BOOST_CHECK_EQUAL(become.eval(), true);  // becomes true
   BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
   BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
   become.onKeywordStateChange("13");
   BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
   BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
   become.onKeywordStateChange("25");
   BOOST_CHECK_EQUAL(become.eval(), true);  // becomes true
   BOOST_CHECK_EQUAL(become.eval(), false); // false if evaluated a second time
   BOOST_CHECK_EQUAL(become.eval(), false); // ...or more
}

//--------------------------------------------------------------
/// \brief	    Int based keyword, <= operator, true at startup
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IntBasedKeywordLowerOrEqualOperatorTrueAtStartup)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\" : \"3\", \"operator\" : \"lowerOrEqual\", \"refValue\" : \"13\" }");
   automation::condition::CBecome become(configuration, lastAcq);

   BOOST_CHECK_EQUAL(become.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(become.eval(), true);
   for (int i = 0; i < 20; ++i)
   {
      become.onKeywordStateChange(boost::lexical_cast<std::string>(i));
      BOOST_CHECK_EQUAL(become.eval(), false); // always false (already signaled)
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
   BOOST_CHECK_THROW(automation::condition::CBecome become(configuration, lastAcq), shared::exception::COutOfRange);
}

BOOST_AUTO_TEST_SUITE_END()