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
#include "../../../../sources/shared/shared/event/Now.h"
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
   boost::posix_time::ptime getNextStopPoint() const { return CEventTimer::getNextStopPoint(); }
   void reset() { CEventTimer::reset(); }
   bool canBeRemoved() const { return CEventTimer::canBeRemoved(); }
   int getId() const { return CEventTimer::getId(); }
};

// Function checking if provided times are close enough
bool isTimeClose(const boost::posix_time::ptime& left, const boost::posix_time::ptime& right, const boost::posix_time::time_duration& tolerance)
{
   const boost::posix_time::time_duration absoluteDuration = (left > right) ? (left - right) : (right - left);
   return (absoluteDuration <= tolerance);
}


//--------------------------------------------------------------
/// \brief	    Nominal case 1
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal1)
{
   const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
   const int evtId = 123456;

   CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
   const boost::posix_time::ptime nextTimePoint(shared::event::now() + period);

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK(isTimeClose(timer.getNextStopPoint(), nextTimePoint, boost::posix_time::millisec(1)));
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

   timer.reset();

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), true);
}

//--------------------------------------------------------------
/// \brief	    Create, stop and restart timer
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(StopAndRestartTimer)
{
   const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
   const int evtId = 123456;

   CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
   boost::posix_time::ptime nextTimePoint(shared::event::now() + period);

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK(isTimeClose(timer.getNextStopPoint(), nextTimePoint, boost::posix_time::millisec(1)));
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

   timer.stop();

   BOOST_CHECK_EQUAL(timer.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), true);

   nextTimePoint = shared::event::now() + period;
   timer.start();

   BOOST_CHECK_EQUAL(timer.getId(), evtId);
   BOOST_CHECK(isTimeClose(timer.getNextStopPoint(), nextTimePoint, boost::posix_time::millisec(1)));
   BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);
}

BOOST_AUTO_TEST_SUITE_END()