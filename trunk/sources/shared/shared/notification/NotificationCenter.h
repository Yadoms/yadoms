#pragma once
#include <shared/Export.h>
#include "NotificationObserver.h"

namespace shared { namespace notification {

   class YADOMS_SHARED_EXPORT CNotificationCenter
   {
   public:
      CNotificationCenter();

      virtual ~CNotificationCenter();
      
      boost::shared_ptr<CNotificationObserver> registerObserver(void * observerKey);
      void unregisterObserver(void * observerKey);

      template<class T>
      void postNotification(const T & notif)
      {
         ObserverCollection::iterator i;
         for (i = m_observers.begin(); i != m_observers.end(); ++i)
            i->second->postNotification(kNotification, notif);
      }

      int waitForNotifications(void * observerKey, const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

      template<class T>
      bool isNotificationTypeOf(void * observerKey)
      {
         ObserverCollection::iterator itObserver = m_observers.find(observerKey);
         if (itObserver != m_observers.end())
         {
            return itObserver->second->isNotificationTypeOf<T>();
         }
         else
            throw shared::exception::CNullReference("Cannot find observer");
      }

      template<class T> 
      T getNotificationData(void * observerKey)
      {
         ObserverCollection::iterator itObserver = m_observers.find(observerKey);
         if (itObserver != m_observers.end())
         {
            return itObserver->second->getNotificationData<T>();
         }
         else
            throw shared::exception::CNullReference("Cannot find observer");
      }

      enum
      {
         kNotification = shared::event::kUserFirstId,
         kPolling,
         kUserFirstId
      };

   private:
      typedef std::map<void*, boost::shared_ptr<CNotificationObserver> > ObserverCollection;
      ObserverCollection m_observers;
   };


} //namespace notification
} //namespace shared
