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
/// \result     No Error
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
/// \result     No Error
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
/// \result     No Error : 1 periodic timer, test 10 occurences 
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NominalEventHandler1TimerPeriodic)
{
   shared::event::CEventHandler evtHandler;
   const boost::posix_time::time_duration period = boost::posix_time::seconds(1);
   const int evtId1 = 1;
   int Counter = 0;
   bool exit=false;
   boost::posix_time::ptime TimePoint       = boost::date_time::not_a_date_time; 
   boost::posix_time::ptime evtId1TimePoint = boost::date_time::not_a_date_time;

   evtHandler.createTimer(evtId1, shared::event::CEventTimer::kPeriodic, period);

   while (!exit)
   {
      switch(evtHandler.waitForEvents(boost::posix_time::milliseconds(3000)))
      {
      case evtId1:
         {
            evtId1TimePoint = TimePoint;
            TimePoint = shared::event::now();

            if (evtId1TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId1TimePoint + period, TimePoint, boost::posix_time::millisec(100)),true);
            Counter++;
            break;
         }
      case shared::event::kTimeout: BOOST_ERROR("TimeOut occured"); break;
      default: BOOST_ERROR("Default case occured !"); break;
      }
      if (Counter >= 10)
         exit = true;
   }
}

//--------------------------------------------------------------
/// \brief	    Test timer with CEventHandler
/// \result     No Error : 10 periodic timers, test 50 occurrences of each. Check time between execution and the average of the periodic time between the 50 executions.
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NominalEventHandler10TimerPeriodic)
{
   shared::event::CEventHandler evtHandler;
   const boost::posix_time::time_duration tolerance    = boost::posix_time::millisec(200);
   const boost::posix_time::time_duration AvgTolerance = boost::posix_time::millisec(20);
   boost::posix_time::ptime TimeReport[2][10];
   const int evtId1 = 1, evtId2 = 2, evtId3 = 3, evtId4 = 4, evtId5 = 5, evtId6 = 6, evtId7 = 7, evtId8 = 8, evtId9 = 9, evtId10 = 10;
   int Counter = 0, IndCounter[10];
   bool exit=false;
   boost::posix_time::ptime TimePoint        = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId1TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId2TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId3TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId4TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId5TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId6TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId7TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId8TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId9TimePoint  = boost::date_time::not_a_date_time;
   boost::posix_time::ptime evtId10TimePoint = boost::date_time::not_a_date_time;

   //Initialization and creation of timers
   for (Counter = 0;Counter < 10;Counter++)
   {
      evtHandler.createTimer(evtId1+Counter, shared::event::CEventTimer::kPeriodic, boost::posix_time::milliseconds(300 + 30*Counter));
      TimeReport[0][Counter] = shared::event::now();
      IndCounter[Counter] = 0;
   }

   Counter = 0;

   while (!exit)
   {
      switch(evtHandler.waitForEvents(boost::posix_time::milliseconds(1000)))
      {
      case evtId1:
         {
            TimePoint = shared::event::now();
            TimeReport[1][0] = TimePoint;

            if (evtId1TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId1TimePoint + boost::posix_time::milliseconds(300), TimePoint, tolerance),true);
            evtId1TimePoint = TimePoint;
            Counter++;
            IndCounter[0]++;
            break;
         }
      case evtId2:
         {
            TimePoint = shared::event::now();
            TimeReport[1][1] = TimePoint;

            if (evtId2TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId2TimePoint + boost::posix_time::milliseconds(300 + 30), TimePoint, tolerance),true);
            evtId2TimePoint = TimePoint;
            Counter++;
            IndCounter[1]++;
            break;
         }
      case evtId3:
         {
            TimePoint = shared::event::now();
            TimeReport[1][2] = TimePoint;

            if (evtId3TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId3TimePoint + boost::posix_time::milliseconds(300 + 60), TimePoint, tolerance),true);
            evtId3TimePoint = TimePoint;
            Counter++;
            IndCounter[2]++;
            break;
         }
      case evtId4:
         {
            TimePoint = shared::event::now();
            TimeReport[1][3] = TimePoint;

            if (evtId4TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId4TimePoint + boost::posix_time::milliseconds(300 + 90), TimePoint, tolerance),true);
            evtId4TimePoint = TimePoint;
            Counter++;
            IndCounter[3]++;
            break;
         }
      case evtId5:
         {
            TimePoint = shared::event::now();
            TimeReport[1][4] = TimePoint;

            if (evtId5TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId5TimePoint + boost::posix_time::milliseconds(300 + 120), TimePoint, tolerance),true);
            evtId5TimePoint = TimePoint;
            Counter++;
            IndCounter[4]++;
            break;
         }
      case evtId6:
         {
            TimePoint = shared::event::now();
            TimeReport[1][5] = TimePoint;

            if (evtId6TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId6TimePoint + boost::posix_time::milliseconds(300 + 150), TimePoint, tolerance),true);
            evtId6TimePoint = TimePoint;
            Counter++;
            IndCounter[5]++;
            break;
         }
      case evtId7:
         {
            TimePoint = shared::event::now();
            TimeReport[1][6] = TimePoint;

            if (evtId7TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId7TimePoint + boost::posix_time::milliseconds(300 + 180), TimePoint, tolerance),true);
            evtId7TimePoint = TimePoint;
            Counter++;
            IndCounter[6]++;
            break;
         }
      case evtId8:
         {
            TimePoint = shared::event::now();
            TimeReport[1][7] = TimePoint;

            if (evtId8TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId8TimePoint + boost::posix_time::milliseconds(300 + 210), TimePoint, tolerance),true);
            evtId8TimePoint = TimePoint;
            Counter++;
            IndCounter[7]++;
            break;
         }
      case evtId9:
         {
            TimePoint = shared::event::now();
            TimeReport[1][8] = TimePoint;

            if (evtId9TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId9TimePoint + boost::posix_time::milliseconds(300 + 240), TimePoint, tolerance),true);
            evtId9TimePoint = TimePoint;
            Counter++;
            IndCounter[8]++;
            break;
         }
      case evtId10:
         {
            TimePoint = shared::event::now();
            TimeReport[1][9] = TimePoint;

            if (evtId10TimePoint != boost::date_time::not_a_date_time)
               BOOST_REQUIRE_EQUAL(isTimeClose(evtId10TimePoint + boost::posix_time::milliseconds(300 + 270), TimePoint, tolerance),true);
            evtId10TimePoint = TimePoint;
            Counter++;
            IndCounter[9]++;
            break;
         }
      case shared::event::kTimeout:
         {
            BOOST_ERROR("TimeOut occured");
            break;
         }
      }
      if (Counter >= 500)
         exit = true;
   }

   for (Counter = 0;Counter < 10;Counter++)
   {
      std::cout << "delta[" << Counter << "]=" << ((TimeReport[1][Counter] - TimeReport[0][Counter]) / IndCounter[Counter] - boost::posix_time::milliseconds(300 + 30* Counter)) << std::endl;
      BOOST_REQUIRE_EQUAL((((TimeReport[1][Counter] - TimeReport[0][Counter]) / IndCounter[Counter])  - boost::posix_time::milliseconds(300 + 30* Counter) < AvgTolerance) ? true : false, true);
   }
}

//--------------------------------------------------------------
/// \brief	    Test timer with CEventHandler
/// \result     No Error : 1 periodic timer, test 10 occurences and a heavy load !
///
/// Start
/// Wait 1500ms
/// waitForEvents( 1 Event already waiting  : Counter = 1)
/// Wait 1500ms
/// waitForEvents( 2 Events already waiting : Only One Retreive) -> Counter = 2
/// Wait 1500ms
/// waitForEvents( 2 Events already waiting : Only One Retreive) -> Counter = 3
/// Wait 1500ms
/// waitForEvents( 3 Events already waiting : Only One Retreive) -> Counter = 4
/// waitForEvents( 2 Events already waiting : Only One Retreive)
/// waitForEvents( 1 Events already waiting : Only One Retreive)
///
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(HeavyLoadEventHandler1TimerPeriodic)
{
   shared::event::CEventHandler evtHandler;
   const boost::posix_time::time_duration period = boost::posix_time::seconds(1);
   const boost::posix_time::time_duration RunningPeriod = boost::posix_time::seconds(5);
   const int evtId1 = 1;
   int Counter = 0;
   bool exit=false;
   boost::posix_time::ptime TimePoint       = boost::date_time::not_a_date_time; 
   boost::posix_time::ptime evtId1TimePoint = boost::date_time::not_a_date_time;
	
   boost::posix_time::ptime start = shared::event::now();

   evtHandler.createTimer(evtId1, shared::event::CEventTimer::kPeriodic, period);

   while (!exit)
   {
      boost::this_thread::sleep(boost::posix_time::milliseconds(1600));

      switch(evtHandler.waitForEvents(boost::posix_time::milliseconds(6000)))
      {
      case evtId1:
         {
			evtId1TimePoint = TimePoint;
			Counter++;
			break;
         }
      case shared::event::kTimeout: BOOST_ERROR("TimeOut occured"); break;
      default: BOOST_ERROR("Default case occured !"); break;
      }
      if (shared::event::now() > start + RunningPeriod)
         exit = true;
   }

	// All events have been captured should have been captured
   BOOST_REQUIRE_EQUAL(Counter, 4);

   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId1); // Must be Event
   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId1); // Must be Event
}

//--------------------------------------------------------------
/// \brief	    Test timer with CEventHandler
/// \result     No Error : 1 periodic timer, timeout, follow by the timer !
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TimerAfterTimeOut)
{
   shared::event::CEventHandler evtHandler;
   const boost::posix_time::time_duration period = boost::posix_time::seconds(2);
   const int evtId1 = 1;
   boost::posix_time::ptime TimePoint       = boost::date_time::not_a_date_time; 

   evtHandler.createTimer(evtId1, shared::event::CEventTimer::kOneShot, period);

   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1500)), shared::event::kTimeout); // TimeOut
   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(5)), evtId1); // Must be Event
}

BOOST_AUTO_TEST_SUITE_END()