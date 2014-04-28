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
   const boost::posix_time::time_duration period(0,0,5);
   const int evtId = 123456;

   shared::event::CEventTimer event(evtId, shared::event::CEventTimer::kOneShot, period);
   const boost::posix_time::ptime nextTimePoint(shared::event::now() + period);

   BOOST_CHECK_EQUAL(event.getId(), evtId);
   BOOST_CHECK(isTimeClose(event.getNextStopPoint(), nextTimePoint, boost::posix_time::millisec(1)));
   BOOST_CHECK_EQUAL(event.canBeRemoved(), false);

   event.reset();

   BOOST_CHECK_EQUAL(event.getId(), evtId);
   BOOST_CHECK_EQUAL(event.getNextStopPoint(), boost::date_time::not_a_date_time);
   BOOST_CHECK_EQUAL(event.canBeRemoved(), true);
}

//--------------------------------------------------------------
/// \brief	    Try to create CEventTimePoint with time point in the past
//--------------------------------------------------------------
/*
BOOST_AUTO_TEST_CASE(timePointInThePast)
{
   const boost::posix_time::ptime timePoint(shared::event::now() - boost::posix_time::seconds(3));
   const int evtId = 123456;
   BOOST_REQUIRE_THROW(shared::event::CEventTimePoint event(evtId, timePoint), shared::exception::CInvalidParameter);
}
*/
BOOST_AUTO_TEST_SUITE_END()