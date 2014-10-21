#include "stdafx.h"
#include "NotificationCenter.h"

namespace shared {   namespace notification{

   CNotificationCenter::CNotificationCenter()
   {

   }

   CNotificationCenter::~CNotificationCenter()
   {

   }
   
   void CNotificationCenter::registerObserver(void * observer)
   {
      m_observers.insert(std::make_pair(observer, CNotificationObserver()));
   }

   void CNotificationCenter::unregisterObserver(void * observer)
   {
      m_observers.erase(observer);
   }

   int CNotificationCenter::waitForNotifications(void * observer, const boost::posix_time::time_duration& timeout)
   {
      if (m_observers.find(observer) != m_observers.end())
      {
         if (!m_observers[observer].isWaiting())
            return m_observers[observer].waitForNotifications(timeout);
         else
            throw shared::exception::CException("Observer is already waiting for events");
      }
      else
         throw shared::exception::CNullReference("Cannot find observer");
   }


} //namespace notification
} //namespace shared
