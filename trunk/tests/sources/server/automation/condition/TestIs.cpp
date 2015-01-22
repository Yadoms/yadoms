#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../../sources/server/automation/condition/Is.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestIs)


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



BOOST_AUTO_TEST_SUITE_END()