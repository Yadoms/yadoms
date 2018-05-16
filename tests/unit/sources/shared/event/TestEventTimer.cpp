#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/event/EventTimer.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

#include "../mock/shared/currentTime/DefaultCurrentTimeMock.h"

shared::event::CEventHandler EvtHandler;

BOOST_AUTO_TEST_SUITE(TestEventTimer)


   // Class used to gain access to protected members of CEventTimer
   class CEventTimerAccessProtectedMembers : public shared::event::CEventTimer
   {
   public:
      explicit CEventTimerAccessProtectedMembers(int eventId, EPeriodicity periodicity = kOneShot,
                                                 const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time)
         : CEventTimer(eventId, periodicity, period)
      {
      }

      virtual ~CEventTimerAccessProtectedMembers()
      {
      }

      boost::posix_time::ptime getNextStopPoint() const override
      {
         return CEventTimer::getNextStopPoint();
      }

      void reset() override
      {
         CEventTimer::reset();
      }

      bool canBeRemoved() const override
      {
         return CEventTimer::canBeRemoved();
      }

      int getId() const override
      {
         return CEventTimer::getId();
      }
   };


   //--------------------------------------------------------------
   /// \brief	    Nominal case
   /// \result     No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(Nominal)
   {
      useTimeMock();

      const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
      const auto evtId = 123456;

      CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period);
      const auto nextTimePoint(shared::currentTime::Provider().now() + period);

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
      useTimeMock();

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
   /// \brief	    Multi starts on one-shot timer
   /// \result     No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(MultiStartsOnOneShotTimer)
   {
      auto timeProviderMock = useTimeMock();

      const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
      const auto evtId = 123456;

      CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kOneShot, period); // Timer started here
      auto nextTimePoint(shared::currentTime::Provider().now() + period);

      BOOST_CHECK_EQUAL(timer.getId(), evtId);
      BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
      BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

      // Restart 2 seconds after first start
      timeProviderMock->sleep(boost::posix_time::seconds(2));

      timer.start(); // Restart timer

      nextTimePoint += boost::posix_time::seconds(2);

      BOOST_CHECK_EQUAL(timer.getId(), evtId);
      BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
      BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);
   }

   //--------------------------------------------------------------
   /// \brief	    Multi starts on periodic timer
   /// \result     No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(MultiStartsOnPeriodicTimer)
   {
      auto timeProviderMock = useTimeMock();

      const boost::posix_time::time_duration period = boost::posix_time::seconds(5);
      const auto evtId = 123456;

      CEventTimerAccessProtectedMembers timer(evtId, shared::event::CEventTimer::kPeriodic, period); // Timer started here
      auto nextTimePoint(shared::currentTime::Provider().now() + period);

      BOOST_CHECK_EQUAL(timer.getId(), evtId);
      BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
      BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);

      // Restart 2 seconds after first start
      timeProviderMock->sleep(boost::posix_time::seconds(2));

      timer.start(); // Restart timer
      // nextTimePoint should not have changed

      BOOST_CHECK_EQUAL(timer.getId(), evtId);
      BOOST_CHECK_EQUAL(timer.getNextStopPoint(), nextTimePoint);
      BOOST_CHECK_EQUAL(timer.canBeRemoved(), false);
   }

   BOOST_AUTO_TEST_CASE(NominalEventHandlerTimerOneShot)
   {
      auto timeProviderMock = useTimeMock();
      shared::event::CEventHandler evtHandler;

      const auto evtId = 123456;
      evtHandler.createTimer(evtId, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(1));

      timeProviderMock->sleep(boost::posix_time::milliseconds(999));
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      timeProviderMock->sleep(boost::posix_time::milliseconds(2));
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId); // One Timer
      timeProviderMock->sleep(boost::posix_time::seconds(5));
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent); // No more Timer
   }

   BOOST_AUTO_TEST_CASE(NominalEventHandlerTimerPeriodic)
   {
      auto timeProviderMock = useTimeMock();
      shared::event::CEventHandler evtHandler;

      const auto evtId = 123456;
      evtHandler.createTimer(evtId, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(1));

      timeProviderMock->sleep(boost::posix_time::milliseconds(1));
      for (auto noOccurence = 1; noOccurence <= 5; ++noOccurence)
      {
         timeProviderMock->sleep(boost::posix_time::milliseconds(998));
         BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
         timeProviderMock->sleep(boost::posix_time::milliseconds(2));
         BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId);
      }
   }

BOOST_AUTO_TEST_SUITE_END()
