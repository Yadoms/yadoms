#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/event/EventTimePoint.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

#include <mock/shared/currentTime/DefaultCurrentTimeMock.h>

BOOST_AUTO_TEST_SUITE(TestTimePoint)

   // Class used to gain access to protected members of CEventTimer
   class CEventTimePointAccessProtectedMembers : public shared::event::CEventTimePoint
   {
   public:
      explicit CEventTimePointAccessProtectedMembers(int eventId, const boost::posix_time::ptime& dateTime = boost::date_time::not_a_date_time)
         : CEventTimePoint(eventId, dateTime)
      {
      }

      virtual ~CEventTimePointAccessProtectedMembers()
      {
      }

      boost::posix_time::ptime getNextStopPoint() const override
      {
         return CEventTimePoint::getNextStopPoint();
      }

      void reset() override
      {
         CEventTimePoint::reset();
      }

      bool canBeRemoved() const override
      {
         return CEventTimePoint::canBeRemoved();
      }

      int getId() const override
      {
         return CEventTimePoint::getId();
      }
   };

   //--------------------------------------------------------------
   /// \brief	    Nominal case
   /// \result     No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(Nominal)
   {
      useTimeMock();
      const auto timePoint(shared::currentTime::Provider().now() + boost::posix_time::seconds(3));
      const auto evtId = 123456;
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
      useTimeMock();
      const auto timePoint(shared::currentTime::Provider().now() - boost::posix_time::seconds(3));
      const auto evtId = 123456;
      BOOST_REQUIRE_THROW(shared::event::CEventTimePoint timer(evtId, timePoint), std::invalid_argument);
   }

   //--------------------------------------------------------------
   /// \brief	    Nominal case + EventHandler
   /// \result     No Error
   //--------------------------------------------------------------
   BOOST_AUTO_TEST_CASE(NominalWithEventHandler)
   {
      auto timeProviderMock = useTimeMock();
      const auto timePoint(shared::currentTime::Provider().now() + boost::posix_time::seconds(1));
      shared::event::CEventHandler evtHandler;
      const auto evtId1 = 123456;

      evtHandler.createTimePoint(evtId1, timePoint);

      timeProviderMock->sleep(boost::posix_time::milliseconds(1000));
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      timeProviderMock->sleep(boost::posix_time::milliseconds(1));
      BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId1);
   }

BOOST_AUTO_TEST_SUITE_END()
