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
         case shared::notification::CNotificationCenter::kTimeout:
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

BOOST_AUTO_TEST_SUITE_END()