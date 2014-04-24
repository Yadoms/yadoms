#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/exception/InvalidParameter.hpp"
#include "../../../../sources/shared/shared/event/Now.h"
#include "../../../../sources/shared/shared/event/EventTimePoint.h"

BOOST_AUTO_TEST_SUITE(TestTimePoint)

//--------------------------------------------------------------
/// \brief	    Nominal case
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal)
{
   const boost::posix_time::ptime timePoint(shared::event::now() + boost::posix_time::seconds(3));
   const int evtId = 123456;
   shared::event::CEventTimePoint event(evtId, timePoint);

	BOOST_CHECK_EQUAL(event.getId(), evtId);
   BOOST_CHECK_EQUAL(event.getNextStopPoint(), timePoint);
	//BOOST_CHECK_EQUAL(event.canBeDetached(), false);

   event.reset();

   BOOST_CHECK_EQUAL(event.getId(), evtId);
   BOOST_CHECK_EQUAL(event.getNextStopPoint(), boost::date_time::not_a_date_time);
   //BOOST_CHECK_EQUAL(event.canBeDetached(), true);
}

//--------------------------------------------------------------
/// \brief	    Try to create CEventTimePoint with time point in the past
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(timePointInThePast)
{
   const boost::posix_time::ptime timePoint(shared::event::now() - boost::posix_time::seconds(3));
   const int evtId = 123456;
   BOOST_REQUIRE_THROW(shared::event::CEventTimePoint event(evtId, timePoint), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_SUITE_END()