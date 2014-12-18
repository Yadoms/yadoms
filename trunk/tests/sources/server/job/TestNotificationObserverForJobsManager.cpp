#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/server/database/entities/Entities.h"
#include "../../../../sources/server/job/NotificationObserverForJobsManager.h"
#include "../../../../sources/server/notifications/NewAcquisitionNotification.h"
#include "../../../../sources/server/notifications/NewDeviceNotification.h"

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestNotificationObserverForJobsManager)

enum
{
   kKeywordNotifierMokeEvent = shared::event::kUserFirstId,
   kConditionRootMokeEvent
};

class CKeywordUpdaterMoke : public job::condition::IKeywordUpdater, public boost::enable_shared_from_this<CKeywordUpdaterMoke>
{
public:
   CKeywordUpdaterMoke(int keywordId, shared::event::CEventHandler& eventHandler) : m_keywordId(keywordId), m_eventHandler(eventHandler) {}
   virtual ~CKeywordUpdaterMoke() {}
   virtual int getKeywordId() const { return m_keywordId; }
   virtual void onKeywordStateChange(const std::string& /*state*/) { m_eventHandler.postEvent<boost::shared_ptr<CKeywordUpdaterMoke> >(kKeywordNotifierMokeEvent, shared_from_this()); }
private:
   const int m_keywordId;
   shared::event::CEventHandler& m_eventHandler;
};


class CConditionRootUpdaterMoke : public job::condition::IConditionRootUpdater, public boost::enable_shared_from_this<CConditionRootUpdaterMoke>
{
public:
   CConditionRootUpdaterMoke(shared::event::CEventHandler& eventHandler) : m_eventHandler(eventHandler) {}
   virtual ~CConditionRootUpdaterMoke() {}
   virtual void onKeywordStateChange() { m_eventHandler.postEvent<boost::shared_ptr<CConditionRootUpdaterMoke> >(kConditionRootMokeEvent, shared_from_this()); }
private:
   shared::event::CEventHandler& m_eventHandler;
};


//--------------------------------------------------------------
/// \brief	    Nominal case
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   const int keywordId = 3;

   boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(keywordId, eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> conditionRootNotifier(new CConditionRootUpdaterMoke(eventHandler));
   observer.registerKeywordUpdater(kw3, conditionRootNotifier);

   // Send notification to notification center
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = keywordId;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword and root condition notifiers, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw3);

   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), conditionRootNotifier);

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    1 condition, several keywords
/// \note keywordID used : 3, 4 (2 times), 5, 8, 9
/// All keyword updaters should be notified
/// The condition should be notified only once
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OneConditionSeveralKeywords)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(3, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw5(new CKeywordUpdaterMoke(5, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_1(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw8(new CKeywordUpdaterMoke(8, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_2(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw9(new CKeywordUpdaterMoke(9, eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> conditionRootNotifier(new CConditionRootUpdaterMoke(eventHandler));
   observer.registerKeywordUpdater(kw3, conditionRootNotifier);
   observer.registerKeywordUpdater(kw5, conditionRootNotifier);
   observer.registerKeywordUpdater(kw4_1, conditionRootNotifier);
   observer.registerKeywordUpdater(kw8, conditionRootNotifier);
   observer.registerKeywordUpdater(kw4_2, conditionRootNotifier);
   observer.registerKeywordUpdater(kw9, conditionRootNotifier);

   // Send notification to notification center ==> 1 acquisition on kw#3
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = 3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword and root condition notifiers, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw3);

   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), conditionRootNotifier);

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);

   // Send notification to notification center ==> 1 acquisition on kw#4
   acq->KeywordId = 4;
   notificationData.reset(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword and root condition notifiers, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_1);
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_2);

   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), conditionRootNotifier);

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    1 condition, several keywords, several acquisition on same keywords
/// \note keywordID used : 3, 4 (2 times), 5, 8, 9
/// All keyword updaters should be notified
/// The condition should be notified only once
/// - 5 acquisitions on kw#3
/// - 3 acquisitions on kw#4
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OneConditionSeveralKeywordsSeveralAcquisistions)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(3, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw5(new CKeywordUpdaterMoke(5, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_1(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw8(new CKeywordUpdaterMoke(8, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_2(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw9(new CKeywordUpdaterMoke(9, eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> conditionRootNotifier(new CConditionRootUpdaterMoke(eventHandler));
   observer.registerKeywordUpdater(kw3, conditionRootNotifier);
   observer.registerKeywordUpdater(kw5, conditionRootNotifier);
   observer.registerKeywordUpdater(kw4_1, conditionRootNotifier);
   observer.registerKeywordUpdater(kw8, conditionRootNotifier);
   observer.registerKeywordUpdater(kw4_2, conditionRootNotifier);
   observer.registerKeywordUpdater(kw9, conditionRootNotifier);

   // Send notifications to notification center ==> 5 acquisitions on kw#3
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = 3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   for (int i = 0 ; i < 5 ; ++i)
      notificationCenter->postNotification(notificationData);

   // Notifications should be received by keyword and root condition notifiers, then transmit to our eventHandler
   for (int i = 0 ; i < 5 ; ++i)
   {
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw3);

      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), conditionRootNotifier);
   }
   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);

   // Send notifications to notification center ==> 3 acquisitions on kw#4
   acq->KeywordId = 4;
   notificationData.reset(new notifications::CNewAcquisitionNotification(acq));
   for (int i = 0 ; i < 3 ; ++i)
      notificationCenter->postNotification(notificationData);

   // Notifications should be received by keyword and root condition notifiers, then transmit to our eventHandler
   for (int i = 0 ; i < 3 ; ++i)
   {
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_1);
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_2);

      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), conditionRootNotifier);
   }

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    several conditions, several keywords, several acquisition on same keywords
/// \note Conditions depends on :
/// - condition1 : kw#3, kw#5
/// - condition2 : kw#4 (2 times), kw#8
/// - condition3 : kw#4, kw#9
/// All keyword updaters should be notified
/// Each condition should be notified only once
/// - 5 acquisitions on kw#3
/// - 3 acquisitions on kw#4
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(SeveralConditionSeveralKeywordsSeveralAcquisistions)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(3, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw5(new CKeywordUpdaterMoke(5, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_1(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw8(new CKeywordUpdaterMoke(8, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_2(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw4_3(new CKeywordUpdaterMoke(4, eventHandler));
   boost::shared_ptr<job::condition::IKeywordUpdater> kw9(new CKeywordUpdaterMoke(9, eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> condition1(new CConditionRootUpdaterMoke(eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> condition2(new CConditionRootUpdaterMoke(eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> condition3(new CConditionRootUpdaterMoke(eventHandler));
   /// condition1 depends on kw#3, kw#5
   observer.registerKeywordUpdater(kw3, condition1);
   observer.registerKeywordUpdater(kw5, condition1);
   /// condition2 depends on kw#4 (2 times), kw#8
   observer.registerKeywordUpdater(kw4_1, condition2);
   observer.registerKeywordUpdater(kw4_2, condition2);
   observer.registerKeywordUpdater(kw8, condition2);
   /// condition3 depends on kw#4, kw#9
   observer.registerKeywordUpdater(kw4_3, condition3);
   observer.registerKeywordUpdater(kw9, condition3);

   // Send notifications to notification center ==> 5 acquisitions on kw#3
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = 3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   for (int i = 0 ; i < 5 ; ++i)
      notificationCenter->postNotification(notificationData);

   // Notifications should be received by keyword and root condition notifiers, then transmit to our eventHandler
   for (int i = 0 ; i < 5 ; ++i)
   {
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw3);

      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), condition1);
   }
   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);

   // Send notifications to notification center ==> 3 acquisitions on kw#4
   acq->KeywordId = 4;
   notificationData.reset(new notifications::CNewAcquisitionNotification(acq));
   for (int i = 0 ; i < 3 ; ++i)
      notificationCenter->postNotification(notificationData);

   // Notifications should be received by keyword and root condition notifiers, then transmit to our eventHandler
   for (int i = 0 ; i < 3 ; ++i)
   {
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_1);
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_2);
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw4_3);

      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), condition2);
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), condition3);
   }
   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);

   // Send notifications to notification center ==> 18 acquisitions on kw#9
   acq->KeywordId = 9;
   notificationData.reset(new notifications::CNewAcquisitionNotification(acq));
   for (int i = 0 ; i < 18 ; ++i)
      notificationCenter->postNotification(notificationData);

   // Notifications should be received by keyword and root condition notifiers, then transmit to our eventHandler
   for (int i = 0 ; i < 18 ; ++i)
   {
      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kKeywordNotifierMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CKeywordUpdaterMoke> >(), kw9);

      BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), kConditionRootMokeEvent);
      BOOST_CHECK_EQUAL(eventHandler.getEventData<boost::shared_ptr<CConditionRootUpdaterMoke> >(), condition3);
   }
   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Bad notification type
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(BadNotificationType)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   const int keywordId = 3;

   boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(keywordId, eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> conditionRootNotifier(new CConditionRootUpdaterMoke(eventHandler));
   observer.registerKeywordUpdater(kw3, conditionRootNotifier);

   // Send "new device" notification to notification center (should be filtered)
   boost::shared_ptr<database::entities::CDevice> newDevice(new database::entities::CDevice);
   boost::shared_ptr<notifications::CNewDeviceNotification> notificationData(new notifications::CNewDeviceNotification(newDevice));
   notificationCenter->postNotification(notificationData);

   // No notification should be received
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Thread safety
//--------------------------------------------------------------
void ThreadSafety_SenderThread(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, int count)
{
   // Send notifications to notification center ==> 5 acquisitions on kw#3
   boost::shared_ptr<database::entities::CAcquisition> acq1(new database::entities::CAcquisition);
   acq1->KeywordId = 3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData1(new notifications::CNewAcquisitionNotification(acq1));
   boost::shared_ptr<database::entities::CAcquisition> acq2(new database::entities::CAcquisition);
   acq2->KeywordId = 8;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData2(new notifications::CNewAcquisitionNotification(acq2));
   for (int i = 0; i < count; ++i)
   {
      notificationCenter->postNotification(notificationData1);
      notificationCenter->postNotification(notificationData2);
   }
}
BOOST_AUTO_TEST_CASE(ThreadSafetyRegister)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::thread(ThreadSafety_SenderThread, notificationCenter, 1000);
   boost::this_thread::sleep(boost::posix_time::milliseconds(0));

   // Register 100 times kW3 for the same condition
   boost::shared_ptr<job::condition::IConditionRootUpdater> conditionRootNotifier(new CConditionRootUpdaterMoke(eventHandler));
   for (int i = 0; i < 100; ++i)
   {
      boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(3, eventHandler));
      observer.registerKeywordUpdater(kw3, conditionRootNotifier);
   }

   // As you can't know how many event will be received, empty queue
   while (eventHandler.waitForEvents(boost::posix_time::milliseconds(500)) != shared::event::kTimeout);

   // Now we have to check the registration count of keyword (expected 100) and condition (expected 1) by sending just one message
   boost::thread(ThreadSafety_SenderThread, notificationCenter, 1);
   bool end = false;
   int keywordRegisterCount = 0, conditionRegisterCount = 0;
   do
   {
      switch (eventHandler.waitForEvents(boost::posix_time::milliseconds(500)))
      {
      case kKeywordNotifierMokeEvent: ++keywordRegisterCount; break;
      case kConditionRootMokeEvent: ++conditionRegisterCount; break;
      case shared::event::kTimeout: end = true; break;
      }
   } while (!end);

   // Check counts
   BOOST_CHECK_EQUAL(keywordRegisterCount, 100);
   BOOST_CHECK_EQUAL(conditionRegisterCount, 1);
}
BOOST_AUTO_TEST_CASE(ThreadSafetyUnregister)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   // Register 200 times kW3 for the same condition
   boost::shared_ptr<job::condition::IKeywordUpdater> kw3(new CKeywordUpdaterMoke(3, eventHandler));
   boost::shared_ptr<job::condition::IConditionRootUpdater> conditionRootNotifier(new CConditionRootUpdaterMoke(eventHandler));
   for (int i = 0; i < 200; ++i)
      observer.registerKeywordUpdater(kw3, conditionRootNotifier);

   // Start sending 1000 events
   boost::thread(ThreadSafety_SenderThread, notificationCenter, 1000);
   boost::this_thread::sleep(boost::posix_time::milliseconds(0));

   // Unregister 100 times kW3 for the same condition
   for (int i = 0; i < 100; ++i)
      observer.unregisterKeywordUpdater(kw3);

   // As you can't know how many event will be received, empty queue
   while (eventHandler.waitForEvents(boost::posix_time::milliseconds(500)) != shared::event::kTimeout);

   // Now we have to check the registration count of keyword (expected 100) and condition (expected 1) by sending just one message
   boost::thread(ThreadSafety_SenderThread, notificationCenter, 1);
   bool end = false;
   int keywordRegisterCount = 0, conditionRegisterCount = 0;
   do
   {
      switch (eventHandler.waitForEvents(boost::posix_time::milliseconds(500)))
      {
      case kKeywordNotifierMokeEvent: ++keywordRegisterCount; break;
      case kConditionRootMokeEvent: ++conditionRegisterCount; break;
      case shared::event::kTimeout: end = true; break;
      }
   } while (!end);

   // Check counts
   BOOST_CHECK_EQUAL(keywordRegisterCount, 100);
   BOOST_CHECK_EQUAL(conditionRegisterCount, 1);
}

BOOST_AUTO_TEST_SUITE_END()