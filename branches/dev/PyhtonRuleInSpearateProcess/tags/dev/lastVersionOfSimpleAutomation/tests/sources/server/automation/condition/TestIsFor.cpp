#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../../sources/server/automation/condition/IsFor.h"

// Mocks
#include "Mocks.hpp"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestIsFor)

//--------------------------------------------------------------
/// \brief	    Timer test
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Timer)
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\": \"3\", \"operator\" : \"equal\", \"refValue\" : \"1\", \"for\" : \"00:00:05.000\" }");
   
   automation::condition::CIsFor isFor(configuration, lastAcq);

   BOOST_CHECK_EQUAL(isFor.getKeywordId(), 3);

   BOOST_CHECK_EQUAL(isFor.eval(), false);
   isFor.onKeywordStateChange("1");
   BOOST_CHECK_EQUAL(isFor.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(4));
   BOOST_CHECK_EQUAL(isFor.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(2));
   BOOST_CHECK_EQUAL(isFor.eval(), true);
   isFor.onKeywordStateChange("0");
   BOOST_CHECK_EQUAL(isFor.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(6));
   isFor.onKeywordStateChange("1");
   BOOST_CHECK_EQUAL(isFor.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(4));
   BOOST_CHECK_EQUAL(isFor.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(2));
   BOOST_CHECK_EQUAL(isFor.eval(), true);
}

//--------------------------------------------------------------
/// \brief	    Condition root notification test
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ConditionRoot)              
{
   boost::shared_ptr<CDatabaseAcquisitionRequesterMock> lastAcq(new CDatabaseAcquisitionRequesterMock());
   lastAcq->m_value = "0";
   const shared::CDataContainer configuration("{ \"keywordId\": \"3\", \"operator\" : \"equal\", \"refValue\" : \"1\", \"for\" : \"00:00:05.000\" }");

   boost::shared_ptr<automation::condition::CIsFor> isFor(new automation::condition::CIsFor(configuration, lastAcq));
   boost::shared_ptr<automation::INotificationObserverForRulesManager> observer(new NotificationObserverForRulesManagerMock);
   boost::shared_ptr<ConditionRootUpdaterMock> conditionRootUpdater(new ConditionRootUpdaterMock);
   isFor->registerToNotificationCenter(observer, conditionRootUpdater);

   BOOST_CHECK_EQUAL(isFor->getKeywordId(), 3);

   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   isFor->onKeywordStateChange("1");
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(4));
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(2));
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), true);
   isFor->onKeywordStateChange("0");
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(6));
   isFor->onKeywordStateChange("1");
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(4));
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(2));
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), true);

   isFor->unregisterFromNotificationCenter(observer);
}

BOOST_AUTO_TEST_SUITE_END()