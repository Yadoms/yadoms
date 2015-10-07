#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/exception/InvalidParameter.hpp"
#include "../../../../sources/shared/shared/event/EventTimePoint.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.hpp"

BOOST_AUTO_TEST_SUITE(TestTimePoint)

// Class used to gain access to protected members of CEventTimer
class CEventTimePointAccessProtectedMembers : public shared::event::CEventTimePoint
{
public:
   CEventTimePointAccessProtectedMembers(int eventId, const boost::posix_time::ptime& dateTime = boost::date_time::not_a_date_time)
      :CEventTimePoint(eventId, dateTime) {}
   virtual ~CEventTimePointAccessProtectedMembers() {}
   boost::posix_time::ptime getNextStopPoint() const { return CEventTimePoint::getNextStopPoint(); }
   void reset() { CEventTimePoint::reset(); }
   bool canBeRemoved() const { return CEventTimePoint::canBeRemoved(); }
   int getId() const { return CEventTimePoint::getId(); }
};

//--------------------------------------------------------------
/// \brief	    Nominal case
/// \result     No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   const boost::posix_time::ptime timePoint(shared::currentTime::Provider::now() + boost::posix_time::seconds(3));
   const int evtId = 123456;
   CEventTimePointAccessProtectedMembers event(evtId, timePoint);

	BOOST_CHECK_EQUAL(event.getId(), evtId);
   BOOST_CHECK_EQUAL(event.getNextStopPoint(), timePoint);
	BOOST_CHECK_EQUAL(event.canBeRemoved(), false);

   event.reset();

   BOOST_CHECK_EQUAL(event.getId(), evtId);
   BOOST_CHECK_EQUAL(event.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(event.canBeRemoved(), true);
}

//--------------------------------------------------------------
/// \brief	    Try to create CEventTimePoint with time point in the past
/// \result     Throw an error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(timePointInThePast)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   const boost::posix_time::ptime timePoint(shared::currentTime::Provider::now() - boost::posix_time::seconds(3));
   const int evtId = 123456;
   BOOST_REQUIRE_THROW(shared::event::CEventTimePoint timer(evtId, timePoint), shared::exception::CInvalidParameter);
}

//--------------------------------------------------------------
/// \brief	    Nominal case + EventHandler
/// \result     No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NominalWithEventHandler)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;
   const boost::posix_time::ptime timePoint(shared::currentTime::Provider::now() + boost::posix_time::seconds(1));
   const int evtId1 = 123456;

   evtHandler.createTimePoint( evtId1, timePoint );

	BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(3)), evtId1); // Must be Event
}

//--------------------------------------------------------------
/// \brief	    Received a Message after a waiting Time
/// \result     No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TimePointBeforeAWait)
{
   boost::shared_ptr<CDefaultCurrentTimeMock> refTime(boost::make_shared<CDefaultCurrentTimeMock>());
   static shared::currentTime::Provider timeProvider(refTime);

   shared::event::CEventHandler evtHandler;
   const boost::posix_time::ptime timePoint(shared::currentTime::Provider::now() + boost::posix_time::seconds(1));
   const int evtId1 = 123456;

   evtHandler.createTimePoint( evtId1, timePoint );

   refTime->sleep(boost::posix_time::seconds(2));

   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId1); // Must be Event
}

BOOST_AUTO_TEST_SUITE_END()