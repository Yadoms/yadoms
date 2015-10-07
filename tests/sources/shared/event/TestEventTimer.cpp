#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/exception/InvalidParameter.hpp"
#include "../../../../sources/shared/shared/event/EventTimer.h"
#include "../../../../sources/shared/shared/currentTime/ICurrentTime.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.hpp"

shared::event::CEventHandler EvtHandler;
static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

BOOST_AUTO_TEST_SUITE(TestEventTimer)


// Class used to gain access to protected members of CEventTimer
class CEventTimerAccessProtectedMembers : public shared::event::CEventTimer
{
public:
   CEventTimerAccessProtectedMembers(int eventId, EPeriodicity periodicity = kOneShot,
      const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time)
      :CEventTimer(eventId, periodicity, period) {}
   virtual ~CEventTimerAccessProtectedMembers() {}
   boost::posix_time::ptime getNextStopPoint() const { return CEventTimer::getNextStopPoint(); }
   void reset() { CEventTimer::reset(); }
   bool canBeRemoved() const { return CEventTimer::canBeRemoved(); }
   int getId() const { return CEventTimer::getId(); }
};


//--------------------------------------------------------------
/// \brief	    Nominal case 1
/// \result     No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal1)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
   const int evtId = 123456;

   CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
   const boost::posix_time::ptime nextTimePoint(shared::currentTime::Provider::now() + period);

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

   timer.reset();

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), true);
}

//--------------------------------------------------------------
/// \brief	    Create, stop and restart timer
/// \result     No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(StopAndRestartTimer)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
   const int evtId = 123456;

   CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
   boost::posix_time::ptime nextTimePoint(shared::currentTime::Provider::now() + period);

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

   timer.stop();

   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), true);

   nextTimePoint = shared::currentTime::Provider::now() + period;
   timer.start();

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);
}

//--------------------------------------------------------------
/// \brief	    Test timer with CEventHandler
/// \result     No Error : Only One Timer Detected
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NominalEventHandlerTimerOneShot)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;
   const boost::posix_time::time_duration period = boost::posix_time::seconds(1);
   const int evtId = 123456;

   evtHandler.createTimer(evtId, shared::event::CEventTimer::kOneShot, period);
   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(2)), evtId); // One Timer
   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(5)), shared::event::kTimeout); // No more Timer
}

//--------------------------------------------------------------
/// \brief	    Test timer with CEventHandler
/// \result     No Error : 1 periodic timer, test 5 occurences 
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NominalEventHandler1TimerPeriodic)
{
   boost::shared_ptr<CDefaultCurrentTimeMock> refTime(boost::make_shared<CDefaultCurrentTimeMock>());
   static shared::currentTime::Provider timeProvider(refTime);

   shared::event::CEventHandler evtHandler;

   const boost::posix_time::time_duration period = boost::posix_time::seconds(1);
   const int evtId = 123456;
   evtHandler.createTimer(evtId, shared::event::CEventTimer::kPeriodic, period);

   for (int noOccurence = 1; noOccurence <= 5; ++noOccurence)
   {
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents((period * noOccurence) - boost::posix_time::milliseconds(1)), shared::event::kTimeout); // Event not yet received
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents((period * noOccurence) + boost::posix_time::milliseconds(1)), evtId); // Event not yet received
   }
}

BOOST_AUTO_TEST_SUITE_END()