#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../../sources/server/automation/condition/At.h"

// Mocks
#include "Mocks.hpp"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestAt)

//--------------------------------------------------------------
/// \brief	    Timer test
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Timer)
{
   boost::posix_time::ptime timePoint(boost::posix_time::microsec_clock::local_time() + boost::posix_time::seconds(5));
   
   automation::condition::CAt at(timePoint);

   BOOST_CHECK_EQUAL(at.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(4));
   BOOST_CHECK_EQUAL(at.eval(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(2));
   BOOST_CHECK_EQUAL(at.eval(), true);
}

//--------------------------------------------------------------
/// \brief	    Condition root notification test
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ConditionRoot)              
{
   boost::posix_time::ptime timePoint(boost::posix_time::microsec_clock::local_time() + boost::posix_time::seconds(5));

   boost::shared_ptr<automation::condition::CAt> at(new automation::condition::CAt(timePoint));
   boost::shared_ptr<ConditionRootUpdaterMock> conditionRootUpdater(new ConditionRootUpdaterMock);
   at->registerToNotificationCenter(boost::shared_ptr<automation::INotificationObserverForRulesManager>(), conditionRootUpdater);

   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(4));
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), false);
   boost::this_thread::sleep_for(boost::chrono::seconds(2));
   BOOST_CHECK_EQUAL(conditionRootUpdater->wasRequested(), true);

   at->unregisterFromNotificationCenter(boost::shared_ptr<automation::INotificationObserverForRulesManager>());
}

BOOST_AUTO_TEST_SUITE_END()