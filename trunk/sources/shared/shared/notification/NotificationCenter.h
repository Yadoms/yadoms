#pragma once
#include <shared/Export.h>
#include "NotificationObserver.h"

namespace shared { namespace notification{

   class YADOMS_SHARED_EXPORT CNotificationCenter
   {
   public:
      CNotificationCenter();

      virtual ~CNotificationCenter();
      
      void registerObserver(void * observer);
      void unregisterObserver(void * observer);

      template<class T>
      void postNotification(const T & notif)
      {
         ObserverCollection::iterator i;
         for (i = m_observers.begin(); i != m_observers.end(); ++i)
            i->second.postNotification(notif);
      }

      int waitForNotifications(void * observer, const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

      template<class T>
      bool isNotificationTypeOf(void * observer)
      {
         if (m_observers.find(observer) != m_observers.end())
         {
            return m_observers[observer].isNotificationTypeOf<T>();
         }
         else
            throw shared::exception::CNullReference("Cannot find observer");
      }

      template<class T> 
      T getNotificationData(void * observer)
      {
         if (m_observers.find(observer) != m_observers.end())
         {
            return m_observers[observer].getNotificationData<T>();
         }
         else
            throw shared::exception::CNullReference("Cannot find observer");
      }

      enum
      {
         kTimeout = shared::event::kTimeout,
         kNotification = shared::event::kUserFirstId,
      };

   private:
      typedef std::map<void*, CNotificationObserver> ObserverCollection;
      ObserverCollection m_observers;
   };


} //namespace notification
} //namespace shared
