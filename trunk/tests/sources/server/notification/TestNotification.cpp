#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/server/notification/NotificationCenter.h"
#include "../../../../sources/server/notification/Helpers.hpp"
#include "../../../../sources/server/notification/action/EventAction.hpp"
#include "../../../../sources/server/notification/action/WaitAction.hpp"

BOOST_AUTO_TEST_SUITE(TestNotification)


static bool ListenerStopRequested;


class CIntNotif
{
public:
   CIntNotif(const int i)
      :m_integer(i)
   {

   }

   virtual ~CIntNotif()
   {

   }

   int m_integer;
};

class CCustomNotif
{
public:
   CCustomNotif(const int i, const std::string & s)
      :m_integer(i), m_string(s)
   {

   }

   virtual ~CCustomNotif()
   {

   }

   int m_integer;
   std::string m_string;
};








void ThreadNotificationListener(boost::shared_ptr<notification::CNotificationCenter> center, int * notifReceivedCount)
{
   try
   {
      
      bool receiveNotificationEnd = false;

      boost::shared_ptr<notification::action::CWaitAction<CCustomNotif> > waitActionCustomNotif(new notification::action::CWaitAction<CCustomNotif>());
      
      //create the acquisition observer
      boost::shared_ptr<notification::basic::CObserver<CCustomNotif> > observer(new notification::basic::CObserver<CCustomNotif>(waitActionCustomNotif));

      //regsiter the observer
      notification::CHelpers::CCustomSubscriber subscriber(observer, center);


      boost::shared_ptr<notification::action::CWaitAction<CIntNotif> > waitActionInt(new notification::action::CWaitAction<CIntNotif>());

      //create the acquisition observer
      boost::shared_ptr<notification::basic::CObserver<CIntNotif> > observerInt(new notification::basic::CObserver<CIntNotif>(waitActionInt));

      //regsiter the observer
      notification::CHelpers::CCustomSubscriber subscriberInt(observerInt, center);

      boost::shared_ptr<CCustomNotif> notifCustom = waitActionCustomNotif->wait(boost::posix_time::seconds(5000));
      if (notifCustom && notifCustom->m_integer == 42 && boost::iequals(notifCustom->m_string, "a custom notif"))
      {
         (*notifReceivedCount)++;
      }

      boost::shared_ptr<CIntNotif> notifInt = waitActionInt->wait(boost::posix_time::seconds(5000));
      if (notifInt && notifInt->m_integer == 666)
      {
         (*notifReceivedCount)++;
      }

      if (*notifReceivedCount == 2)
         receiveNotificationEnd = true;

      
   }
   catch (...)
   {

   }
}

BOOST_AUTO_TEST_CASE(Notification_3Clients)
{
   boost::shared_ptr<notification::CNotificationCenter> center(new notification::CNotificationCenter());

   boost::shared_ptr<CIntNotif> notificationInt(new CIntNotif(666));
   boost::shared_ptr<CCustomNotif> notifCustom(new CCustomNotif(42, "a custom notif"));


   int notifReceivedCountL1 = 0;
   int notifReceivedCountL2 = 0;
   int notifReceivedCountL3 = 0;

   //create listeners
   boost::thread listener1(ThreadNotificationListener, center, &notifReceivedCountL1);
   boost::thread listener2(ThreadNotificationListener, center, &notifReceivedCountL2);
   boost::thread listener3(ThreadNotificationListener, center, &notifReceivedCountL3);

   //wait some time to provide time for listeners
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   //post notification
   notification::CHelpers::postNotification(notificationInt, center);
   notification::CHelpers::postNotification(notifCustom, center);

   //wait some time to provide time for listeners
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   //ask threads to stop
   ListenerStopRequested = true;

   //wait for all listeners
   listener1.join();
   listener2.join();
   listener3.join();

   BOOST_CHECK_EQUAL(notifReceivedCountL1, 2);
   BOOST_CHECK_EQUAL(notifReceivedCountL2, 2);
   BOOST_CHECK_EQUAL(notifReceivedCountL3, 2);
}


void ThreadPoster(boost::shared_ptr<notification::CNotificationCenter> center, int nbPosts)
{
   //wait some time to provide time for listener
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   boost::shared_ptr<CIntNotif> notificationInt(new CIntNotif(666));
   boost::shared_ptr<CCustomNotif> notifCustom(new CCustomNotif(42, "a custom notif"));

   for (int i = 0 ; i < nbPosts ; ++i)
   {
      notification::CHelpers::postNotification(notificationInt, center);
      notification::CHelpers::postNotification(notifCustom, center);
   }
}

//BOOST_AUTO_TEST_CASE(Notification_1ClientNPosters)
//{
//   boost::shared_ptr<notification::CNotificationCenter> center(new notification::CNotificationCenter());
//
//   static const int nbPosters = 100;
//   static const int nbPostsByPoster = 1000;
//
//   boost::shared_ptr<boost::thread> posters[nbPosters];
//   int intNotifReceivedCount = 0;
//   int curtomNotifReceivedCount = 0;
//
//
//   //create posters
//   for (size_t i = 0 ; i < nbPosters ; ++i)
//      posters[i].reset(new boost::thread(ThreadPoster, center, nbPostsByPoster));
//
//   bool allPostersAreOff = false;
//   while (!allPostersAreOff)
//   {
//      switch (center.waitForNotifications(oberserverKey, boost::posix_time::milliseconds(500)))
//      {
//      case shared::notification::CNotificationCenter::kNotification:
//         {
//            if (center.isNotificationTypeOf<int>(oberserverKey))
//            {
//               int notif = center.getNotificationData<int>(oberserverKey);
//               if (notif == 666)
//               {
//                  ++ intNotifReceivedCount;
//               }
//            }
//            else if (center.isNotificationTypeOf<CCustomNotif>(oberserverKey))
//            {
//               CCustomNotif notif = center.getNotificationData<CCustomNotif>(oberserverKey);
//               if (notif.m_integer == 42 && boost::iequals(notif.m_string, "a custom notif") )
//               {
//                  ++ curtomNotifReceivedCount;
//               }
//            }
//            break;
//         }
//
//      case shared::event::kTimeout:
//         {
//            // Check if all posters finished their job
//            allPostersAreOff = true;
//            for (size_t i = 0 ; allPostersAreOff && i < nbPosters ; ++i)
//               if (!posters[i]->timed_join(boost::posix_time::milliseconds(0))) allPostersAreOff = false;
//            break;
//         }
//
//      default:
//         BOOST_FAIL("Unexpected waitForNotifications return code");
//         break;
//      }
//   }
//
//   center.unregisterObserver(oberserverKey);
//
//   BOOST_CHECK_EQUAL(intNotifReceivedCount, nbPosters * nbPostsByPoster);
//   BOOST_CHECK_EQUAL(curtomNotifReceivedCount, nbPosters * nbPostsByPoster);
//}

BOOST_AUTO_TEST_SUITE_END()