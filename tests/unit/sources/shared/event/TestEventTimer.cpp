#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/event/EventTimer.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

shared::event::CEventHandler EvtHandler;

BOOST_AUTO_TEST_SUITE(TestEventTimer)


// Class used to gain access to protected members of CEventTimer
class CEventTimerAccessProtectedMembers : public shared::event::CEventTimer
{
public:
   CEventTimerAccessProtectedMembers(int eventId, EPeriodicity periodicity = kOneShot,
      const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time)
      :CEventTimer(eventId, periodicity, period) {}
   virtual ~CEventTimerAccessProtectedMembers() {}
   boost::posix_time::ptime getNextStopPoint() const override
   { return CEventTimer::getNextStopPoint(); }
   void reset() override
   { CEventTimer::reset(); }
   bool canBeRemoved() const override
   { return CEventTimer::canBeRemoved(); }
   int getId() const override
   { return CEventTimer::getId(); }
};


//--------------------------------------------------------------
/// \brief	    Nominal case 1
/// \result     No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal1)
{
   const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
   const auto evtId = 123456;

   CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
   const auto nextTimePoint(shared::currentTime::Provider().now() + period);

   BOOST_CHECK_EQUAL(timer.getId(), evtId);

   //as timer var and nextTimePoint are not created exactly at the same time (few cpu instuctions; and may be interruptible by others processes)
   //checking timer.getNextStopPoint() equals to nextTimePoint may fails.
   //so allow a time diff
   const auto diff = (timer.getNextStopPoint() - nextTimePoint).total_milliseconds();
   BOOST_CHECK_LE(abs(diff), 50); //assume max 50 ms
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
   const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
   const auto evtId = 123456;

   CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
   auto nextTimePoint(shared::currentTime::Provider().now() + period);

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

   timer.stop();

   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), true);

   nextTimePoint = shared::currentTime::Provider().now() + period;
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
   shared::event::CEventHandler evtHandler;

   const boost::posix_time::time_duration period = boost::posix_time::seconds(1);
   const int evtId = 123456;
   evtHandler.createTimer(evtId, shared::event::CEventTimer::kPeriodic, period);

   for (int noOccurence = 1; noOccurence <= 5; ++noOccurence)
   {
	  BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(period - boost::posix_time::milliseconds(10)), shared::event::kTimeout); // Event not yet received
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(period + boost::posix_time::milliseconds(10)), evtId); // Event not yet received
   }
}

BOOST_AUTO_TEST_SUITE_END()