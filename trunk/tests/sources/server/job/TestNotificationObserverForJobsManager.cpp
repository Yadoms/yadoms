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

class CKeywordNotifierMoke : public job::condition::IKeywordNotifier
{
public:
   enum { kKeywordNotifierMokeEvent = shared::event::kUserFirstId } ;
   CKeywordNotifierMoke(int keywordId, shared::event::CEventHandler& eventHandler) : m_keywordId(keywordId), m_eventHandler(eventHandler) {}
   virtual ~CKeywordNotifierMoke() {}
   virtual int getKeywordId() const { return m_keywordId; }
   virtual void onNotify() const { m_eventHandler.postEvent<int>(kKeywordNotifierMokeEvent, m_keywordId); }
private:
   const int m_keywordId;
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

   boost::shared_ptr<job::condition::IKeywordNotifier> kw3(new CKeywordNotifierMoke(3, eventHandler));
   observer.registerKeyword(kw3);

   // Send notification to notification center
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = kw3->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword notifier, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw3->getKeywordId());

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Several registers of same notifier
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(SeveralRegistersOfSameNotifier)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::shared_ptr<job::condition::IKeywordNotifier> kw3(new CKeywordNotifierMoke(3, eventHandler));
   boost::shared_ptr<job::condition::IKeywordNotifier> kw3bis = kw3;
   boost::shared_ptr<job::condition::IKeywordNotifier> kw3bister = kw3bis;
   observer.registerKeyword(kw3);
   observer.registerKeyword(kw3bis);
   observer.registerKeyword(kw3bister);

   // Send notification to notification center
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = kw3->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword notifier, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw3->getKeywordId());

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

   boost::shared_ptr<job::condition::IKeywordNotifier> kw3(new CKeywordNotifierMoke(3, eventHandler));
   boost::shared_ptr<job::condition::IKeywordNotifier> kw3bis = kw3;
   boost::shared_ptr<job::condition::IKeywordNotifier> kw3bister = kw3bis;
   observer.registerKeyword(kw3);
   observer.registerKeyword(kw3bis);
   observer.registerKeyword(kw3bister);

   // Send "new device" notification to notification center (should be filtered)
   boost::shared_ptr<database::entities::CDevice> newDevice(new database::entities::CDevice);
   boost::shared_ptr<notifications::CNewDeviceNotification> notificationData(new notifications::CNewDeviceNotification(newDevice));
   notificationCenter->postNotification(notificationData);

   // No notification should be received
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Several notifications, one observer
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(SeveralNotificationsOneObserver)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::shared_ptr<job::condition::IKeywordNotifier> kw3(new CKeywordNotifierMoke(3, eventHandler));
   observer.registerKeyword(kw3);

   // Send notification to notification center
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = kw3->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);
   notificationCenter->postNotification(notificationData);
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword notifier, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw3->getKeywordId());
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw3->getKeywordId());
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw3->getKeywordId());

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Several keywords, one observer
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(SeveralKeywordsOneObserver)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler;
   job::CNotificationObserverForJobsManager observer(notificationCenter);

   boost::shared_ptr<job::condition::IKeywordNotifier> kw3(new CKeywordNotifierMoke(3, eventHandler)); observer.registerKeyword(kw3);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw8(new CKeywordNotifierMoke(8, eventHandler)); observer.registerKeyword(kw8);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw15(new CKeywordNotifierMoke(15, eventHandler)); observer.registerKeyword(kw15);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw21(new CKeywordNotifierMoke(21, eventHandler)); observer.registerKeyword(kw21);

   // Send notifications to notification center
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = kw3->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   boost::shared_ptr<database::entities::CAcquisition> acq2(new database::entities::CAcquisition);
   acq2->KeywordId = kw8->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData2(new notifications::CNewAcquisitionNotification(acq2));
   notificationCenter->postNotification(notificationData2);

   boost::shared_ptr<database::entities::CAcquisition> acq3(new database::entities::CAcquisition);
   acq3->KeywordId = kw15->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData3(new notifications::CNewAcquisitionNotification(acq3));
   notificationCenter->postNotification(notificationData3);

   boost::shared_ptr<database::entities::CAcquisition> acq4(new database::entities::CAcquisition);
   acq4->KeywordId = kw21->getKeywordId();
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData4(new notifications::CNewAcquisitionNotification(acq4));
   notificationCenter->postNotification(notificationData4);

   // Notifications should be received on the same keyword notifier, then transmit to our eventHandler
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw3->getKeywordId());
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw8->getKeywordId());
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw15->getKeywordId());
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler.getEventData<int>(), kw21->getKeywordId());

   // No more event
   BOOST_CHECK_EQUAL(eventHandler.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    One keyword Id, several observers
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OneKeywordSeveralObservers)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   shared::event::CEventHandler eventHandler1, eventHandler2, eventHandler3;
   job::CNotificationObserverForJobsManager observer1(notificationCenter);
   job::CNotificationObserverForJobsManager observer2(notificationCenter);
   job::CNotificationObserverForJobsManager observer3(notificationCenter);

   const int keywordId = 25;
   boost::shared_ptr<job::condition::IKeywordNotifier> kw1(new CKeywordNotifierMoke(keywordId, eventHandler1)); observer1.registerKeyword(kw1);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw2(new CKeywordNotifierMoke(keywordId, eventHandler2)); observer2.registerKeyword(kw2);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw3(new CKeywordNotifierMoke(keywordId, eventHandler3)); observer3.registerKeyword(kw3);

   // Send notification on the keywordId to notification center
   boost::shared_ptr<database::entities::CAcquisition> acq(new database::entities::CAcquisition);
   acq->KeywordId = keywordId;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData(new notifications::CNewAcquisitionNotification(acq));
   notificationCenter->postNotification(notificationData);

   // Notification should be received by keyword notifier, then transmit to each eventHandler
   BOOST_CHECK_EQUAL(eventHandler1.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler1.getEventData<int>(), keywordId);
   BOOST_CHECK_EQUAL(eventHandler1.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout); // No more event
   BOOST_CHECK_EQUAL(eventHandler2.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler2.getEventData<int>(), keywordId);
   BOOST_CHECK_EQUAL(eventHandler2.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout); // No more event
   BOOST_CHECK_EQUAL(eventHandler3.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
   BOOST_CHECK_EQUAL(eventHandler3.getEventData<int>(), keywordId);
   BOOST_CHECK_EQUAL(eventHandler3.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout); // No more event
}

//--------------------------------------------------------------
/// \brief	    Several keywords, several observers, several notifications
/// \note 3 keywordID are used : 25, 37, 72
/// 3 observers :
/// - observerA is listening the 3 keywordID
/// - observerB is listening keywordID 25 and 72
/// - observerC is listening keywordID 37
/// Then notifications are sent :
/// - 2 notifications on keywordId 25
/// - 1 notifications on keywordId 37
/// - 4 notifications on keywordId 72
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(SeveralKeywordsSeveralObserversSeveralNotifications)
{
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter());
   // Use a eventHandler for each keywordNotifier to be sure that notification is received by the correct keywordNotifier
   shared::event::CEventHandler eventHandler1, eventHandler2, eventHandler3;
   job::CNotificationObserverForJobsManager observerA(notificationCenter);
   job::CNotificationObserverForJobsManager observerB(notificationCenter);
   job::CNotificationObserverForJobsManager observerC(notificationCenter);

   // The 3 keywordID : 25, 37, 72
   const int keywordId1 = 25, keywordId2 = 37, keywordId3 = 72;
   // observerA is listening keywordID 25, 37, 72
   boost::shared_ptr<job::condition::IKeywordNotifier> kw1_1(new CKeywordNotifierMoke(keywordId1, eventHandler1)); observerA.registerKeyword(kw1_1);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw1_2(new CKeywordNotifierMoke(keywordId2, eventHandler1)); observerA.registerKeyword(kw1_2);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw1_3(new CKeywordNotifierMoke(keywordId3, eventHandler1)); observerA.registerKeyword(kw1_3);
   // observerB is listening keywordID 25 and 72
   boost::shared_ptr<job::condition::IKeywordNotifier> kw2_1(new CKeywordNotifierMoke(keywordId1, eventHandler2)); observerB.registerKeyword(kw2_1);
   boost::shared_ptr<job::condition::IKeywordNotifier> kw2_2(new CKeywordNotifierMoke(keywordId3, eventHandler2)); observerB.registerKeyword(kw2_2);
   // observerC is listening keywordID 37
   boost::shared_ptr<job::condition::IKeywordNotifier> kw3_1(new CKeywordNotifierMoke(keywordId2, eventHandler3)); observerC.registerKeyword(kw3_1);

   // Send notifications to notification center
   // - 2 notifications on keywordId1(25)
   boost::shared_ptr<database::entities::CAcquisition> acq1_1(new database::entities::CAcquisition);
   acq1_1->KeywordId = keywordId1;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData1_1(new notifications::CNewAcquisitionNotification(acq1_1));
   notificationCenter->postNotification(notificationData1_1);
   boost::shared_ptr<database::entities::CAcquisition> acq1_2(new database::entities::CAcquisition);
   acq1_2->KeywordId = keywordId1;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData1_2(new notifications::CNewAcquisitionNotification(acq1_2));
   notificationCenter->postNotification(notificationData1_2);
   // - 1 notification on keywordId2(37)
   boost::shared_ptr<database::entities::CAcquisition> acq2(new database::entities::CAcquisition);
   acq2->KeywordId = keywordId2;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData2(new notifications::CNewAcquisitionNotification(acq2));
   notificationCenter->postNotification(notificationData2);
   // - 4 notifications on keywordId3(72)
   boost::shared_ptr<database::entities::CAcquisition> acq3_1(new database::entities::CAcquisition);
   acq3_1->KeywordId = keywordId3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData3_1(new notifications::CNewAcquisitionNotification(acq3_1));
   notificationCenter->postNotification(notificationData3_1);
   boost::shared_ptr<database::entities::CAcquisition> acq3_2(new database::entities::CAcquisition);
   acq3_2->KeywordId = keywordId3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData3_2(new notifications::CNewAcquisitionNotification(acq3_2));
   notificationCenter->postNotification(notificationData3_2);
   boost::shared_ptr<database::entities::CAcquisition> acq3_3(new database::entities::CAcquisition);
   acq3_3->KeywordId = keywordId3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData3_3(new notifications::CNewAcquisitionNotification(acq3_3));
   notificationCenter->postNotification(notificationData3_3);
   boost::shared_ptr<database::entities::CAcquisition> acq3_4(new database::entities::CAcquisition);
   acq3_4->KeywordId = keywordId3;
   boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData3_4(new notifications::CNewAcquisitionNotification(acq3_4));
   notificationCenter->postNotification(notificationData3_4);

   // Notifications should be received by keyword notifier, then transmit to each eventHandler
   // - observerA(eventHandler1) should receive :
   //   - 2 notifications on keywordId1(25)
   //   - 1 notification on keywordId2(37)
   //   - 4 notifications on keywordId3(72)
   {
      std::map<int /*keywordId*/, int/*notificationCount*/> notificationsReceived;
      for (int notificationCount = 0 ; notificationCount < (2+1+4) ; ++notificationCount)
      {
         BOOST_CHECK_EQUAL(eventHandler1.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
         int receivedKeywordId = eventHandler1.getEventData<int>();
         if (notificationsReceived.find(receivedKeywordId) == notificationsReceived.end())
            notificationsReceived[eventHandler1.getEventData<int>()] = 1;
         else
            notificationsReceived[eventHandler1.getEventData<int>()] ++;
      }
      BOOST_CHECK_EQUAL(eventHandler1.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout); // No more event
      BOOST_CHECK(notificationsReceived.find(25) != notificationsReceived.end());
      BOOST_CHECK_EQUAL(notificationsReceived[25], 2);
      BOOST_CHECK(notificationsReceived.find(37) != notificationsReceived.end());
      BOOST_CHECK_EQUAL(notificationsReceived[37], 1);
      BOOST_CHECK(notificationsReceived.find(72) != notificationsReceived.end());
      BOOST_CHECK_EQUAL(notificationsReceived[72], 4);
   }
   // - observerB(eventHandler2) should receive :
   //   - 2 notifications on keywordId1(25)
   //   - 4 notifications on keywordId3(72)
   {
      std::map<int /*keywordId*/, int/*notificationCount*/> notificationsReceived;
      for (int notificationCount = 0 ; notificationCount < (2+4) ; ++notificationCount)
      {
         BOOST_CHECK_EQUAL(eventHandler2.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
         int receivedKeywordId = eventHandler2.getEventData<int>();
         if (notificationsReceived.find(receivedKeywordId) == notificationsReceived.end())
            notificationsReceived[eventHandler2.getEventData<int>()] = 1;
         else
            notificationsReceived[eventHandler2.getEventData<int>()] ++;
      }
      BOOST_CHECK_EQUAL(eventHandler2.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout); // No more event
      BOOST_CHECK(notificationsReceived.find(25) != notificationsReceived.end());
      BOOST_CHECK_EQUAL(notificationsReceived[25], 2);
      BOOST_CHECK(notificationsReceived.find(37) == notificationsReceived.end());
      BOOST_CHECK(notificationsReceived.find(72) != notificationsReceived.end());
      BOOST_CHECK_EQUAL(notificationsReceived[72], 4);
   }
   // - observerC(eventHandler3) should receive :
   //   - 1 notification on keywordId2(37)
   {
      std::map<int /*keywordId*/, int/*notificationCount*/> notificationsReceived;
      for (int notificationCount = 0 ; notificationCount < 1 ; ++notificationCount)
      {
         BOOST_CHECK_EQUAL(eventHandler3.waitForEvents(boost::posix_time::milliseconds(500)), CKeywordNotifierMoke::kKeywordNotifierMokeEvent);
         int receivedKeywordId = eventHandler3.getEventData<int>();
         if (notificationsReceived.find(receivedKeywordId) == notificationsReceived.end())
            notificationsReceived[eventHandler3.getEventData<int>()] = 1;
         else
            notificationsReceived[eventHandler3.getEventData<int>()] ++;
      }
      BOOST_CHECK_EQUAL(eventHandler3.waitForEvents(boost::posix_time::milliseconds(500)), shared::event::kTimeout); // No more event
      BOOST_CHECK(notificationsReceived.find(25) == notificationsReceived.end());
      BOOST_CHECK(notificationsReceived.find(37) != notificationsReceived.end());
      BOOST_CHECK_EQUAL(notificationsReceived[37], 1);
      BOOST_CHECK(notificationsReceived.find(72) == notificationsReceived.end());
   }
}


BOOST_AUTO_TEST_SUITE_END()