#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/notification/NotificationCenter.h"

BOOST_AUTO_TEST_SUITE(TestNotification)


static bool ListenerStopRequested;


class CCustomNotif
{
public:
   CCustomNotif(const int i, const std::string & s)
      :m_integer(i), m_string(s)
   {

   }


   int m_integer;
   std::string m_string;
};








void ThreadNotificationListener(shared::notification::CNotificationCenter * center, int * notifReceivedCount)
{
   try
   {
      
      bool receiveNotificationEnd = false;
      center->registerObserver(notifReceivedCount);

      while (!ListenerStopRequested && !receiveNotificationEnd)
      {
         switch (center->waitForNotifications(notifReceivedCount, boost::posix_time::seconds(5000)))
         {
         case shared::notification::CNotificationCenter::kNotification:
         {
            if (center->isNotificationTypeOf<int>(notifReceivedCount))
            {
               int notif = center->getNotificationData<int>(notifReceivedCount);
               if (notif == 666)
               {
                  (*notifReceivedCount)++;
               }
            }
            else if (center->isNotificationTypeOf<CCustomNotif>(notifReceivedCount))
            {
               CCustomNotif notif = center->getNotificationData<CCustomNotif>(notifReceivedCount);
               if (notif.m_integer == 42 && boost::iequals(notif.m_string, "a custom notif") )
               {
                  (*notifReceivedCount)++;
               }
            }

            if (*notifReceivedCount == 2)
               receiveNotificationEnd = true;

            break;
         }
         case shared::notification::CNotificationCenter::kPolling:
         {
            receiveNotificationEnd = true;
            break;
         }

         default:
            break;
         }
      }
   }
   catch (...)
   {

   }
   center->unregisterObserver(&notifReceivedCount);
}

BOOST_AUTO_TEST_CASE(Notification_3Clients)
{
   shared::notification::CNotificationCenter center;

   int notificationInt = 666;
   CCustomNotif notifCustom(42, "a custom notif");

   int notifReceivedCountL1 = 0;
   int notifReceivedCountL2 = 0;
   int notifReceivedCountL3 = 0;

   //create listeners
   boost::thread listener1(ThreadNotificationListener, &center, &notifReceivedCountL1);
   boost::thread listener2(ThreadNotificationListener, &center, &notifReceivedCountL2);
   boost::thread listener3(ThreadNotificationListener, &center, &notifReceivedCountL3);

   //wait some time to provide time for listeners
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   //post notification
   center.postNotification<int>(notificationInt);
   center.postNotification<CCustomNotif>(notifCustom);

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


void ThreadPoster(shared::notification::CNotificationCenter * center, int nbPosts)
{
   //wait some time to provide time for listener
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   int notificationInt = 666;
   CCustomNotif notifCustom(42, "a custom notif");

   for (int i = 0 ; i < nbPosts ; ++i)
   {
      center->postNotification<int>(notificationInt);
      center->postNotification<CCustomNotif>(notifCustom);
   }
}

BOOST_AUTO_TEST_CASE(Notification_1ClientNPosters)
{
   shared::notification::CNotificationCenter center;

   static const int nbPosters = 100;
   static const int nbPostsByPoster = 1000;
   static void* oberserverKey = NULL;
   boost::shared_ptr<boost::thread> posters[nbPosters];
   int intNotifReceivedCount = 0;
   int curtomNotifReceivedCount = 0;

   center.registerObserver(oberserverKey);

   //create posters
   for (size_t i = 0 ; i < nbPosters ; ++i)
      posters[i].reset(new boost::thread(ThreadPoster, &center, nbPostsByPoster));

   bool allPostersAreOff = false;
   while (!allPostersAreOff)
   {
      switch (center.waitForNotifications(oberserverKey, boost::posix_time::milliseconds(500)))
      {
      case shared::notification::CNotificationCenter::kNotification:
         {
            if (center.isNotificationTypeOf<int>(oberserverKey))
            {
               int notif = center.getNotificationData<int>(oberserverKey);
               if (notif == 666)
               {
                  ++ intNotifReceivedCount;
               }
            }
            else if (center.isNotificationTypeOf<CCustomNotif>(oberserverKey))
            {
               CCustomNotif notif = center.getNotificationData<CCustomNotif>(oberserverKey);
               if (notif.m_integer == 42 && boost::iequals(notif.m_string, "a custom notif") )
               {
                  ++ curtomNotifReceivedCount;
               }
            }
            break;
         }
      case shared::notification::CNotificationCenter::kPolling:
         {
            // Check if all posters finished their job
            allPostersAreOff = true;
            for (size_t i = 0 ; allPostersAreOff && i < nbPosters ; ++i)
               if (!posters[i]->timed_join(boost::posix_time::milliseconds(0))) allPostersAreOff = false;
            break;
         }

      default:
         BOOST_FAIL("Unexpected waitForNotifications return code");
         break;
      }
   }

   center.unregisterObserver(oberserverKey);

   BOOST_CHECK_EQUAL(intNotifReceivedCount, nbPosters * nbPostsByPoster);
   BOOST_CHECK_EQUAL(curtomNotifReceivedCount, nbPosters * nbPostsByPoster);
}

BOOST_AUTO_TEST_SUITE_END()