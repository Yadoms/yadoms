#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include "../../../../sources/server/notification/NotificationCenter.h"
#include "../../../../sources/server/notification/Helpers.hpp"
#include "../../../../sources/server/notification/action/WaitAction.hpp"

BOOST_AUTO_TEST_SUITE(TestNotification)

   static bool ListenerStopRequested;


   class CIntNotif
   {
   public:
      explicit CIntNotif(const int i)
         : m_integer(i)
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
      CCustomNotif(const int i, const std::string& s)
         : m_integer(i), m_string(s)
      {
      }

      virtual ~CCustomNotif()
      {
      }

      int m_integer;
      std::string m_string;
   };


   void ThreadNotificationListener(boost::shared_ptr<notification::CNotificationCenter> center, int* notifReceivedCount)
   {
      try
      {
         auto waitActionCustomNotif(boost::make_shared<notification::action::CWaitAction<CCustomNotif> >());

         //create the acquisition observer
         auto observer(boost::make_shared<notification::basic::CObserver<CCustomNotif> >(waitActionCustomNotif));

         //regsiter the observer
         notification::CHelpers::CCustomSubscriber subscriber(observer, center);


         auto waitActionInt(boost::make_shared<notification::action::CWaitAction<CIntNotif> >());

         //create the acquisition observer
         auto observerInt(boost::make_shared<notification::basic::CObserver<CIntNotif> >(waitActionInt));

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
      }
      catch (...)
      {
      }
   }

   BOOST_AUTO_TEST_CASE(Notification_3Clients)
   {
      auto center(boost::make_shared<notification::CNotificationCenter>());

      auto notificationInt(boost::make_shared<CIntNotif>(666));
      auto notifCustom(boost::make_shared<CCustomNotif>(42, "a custom notif"));


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

      BOOST_CHECK_EQUAL(notifReceivedCountL1, 2) ;
      BOOST_CHECK_EQUAL(notifReceivedCountL2, 2) ;
      BOOST_CHECK_EQUAL(notifReceivedCountL3, 2) ;
   }


   void ThreadPoster(boost::shared_ptr<notification::CNotificationCenter> center, int nbPosts)
   {
      //wait some time to provide time for listener
      boost::this_thread::sleep(boost::posix_time::milliseconds(500));

      auto notificationInt(boost::make_shared<CIntNotif>(666));
      auto notifCustom(boost::make_shared<CCustomNotif>(42, "a custom notif"));

      for (int i = 0; i < nbPosts; ++i)
      {
         notification::CHelpers::postNotification(notificationInt, center);
         notification::CHelpers::postNotification(notifCustom, center);
      }
   }

   BOOST_AUTO_TEST_SUITE_END()

