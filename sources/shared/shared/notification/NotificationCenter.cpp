#include "stdafx.h"
#include "NotificationCenter.h"

namespace shared { namespace notification {

   CNotificationCenter::CNotificationCenter()
   {

   }

   CNotificationCenter::~CNotificationCenter()
   {

   }
   
   boost::shared_ptr<CNotificationObserver> CNotificationCenter::registerObserver(void * observerKey)
   {
      boost::shared_ptr<CNotificationObserver> observer(new CNotificationObserver());
      m_observers.insert(std::make_pair(observerKey, observer));
      return observer;
   }

   void CNotificationCenter::unregisterObserver(void * observerKey)
   {
      m_observers.erase(observerKey);
   }

   int CNotificationCenter::waitForNotifications(void * observerKey, const boost::posix_time::time_duration& timeout)
   {
      ObserverCollection::iterator itObserver = m_observers.find(observerKey);
      if (itObserver == m_observers.end())
         throw shared::exception::CNullReference("Cannot find observer");

      return itObserver->second->waitForNotifications(timeout);
   }


} //namespace notification
} //namespace shared
