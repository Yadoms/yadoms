#include "stdafx.h"
#include "NotificationCenter.h"
#include "basic/Notification.hpp"
#include <shared/exception/NullReference.hpp>

namespace notification {

   CNotificationCenter::CNotificationCenter()
   {
   }

   CNotificationCenter::~CNotificationCenter()
   {
   }

   void CNotificationCenter::postNotification(const boost::shared_ptr<INotification> notification)
   {
      if (notification)
      {
         //make the observers list copy, and release mutex
         boost::mutex::scoped_lock lock(m_mutex);
         std::vector< boost::shared_ptr< IObserver > > localList(m_observers);
         lock.unlock();

         //do post notification (may be long)
         std::vector< boost::shared_ptr< IObserver > >::iterator i;
         for (i = localList.begin(); i != localList.end(); ++i)
         {
            (*i)->observe(notification);
         }
      }
      else
      {
         throw shared::exception::CNullReference("Notification can not be null");
      }
   }



   void CNotificationCenter::subscribeObserver(boost::shared_ptr< IObserver > observer)
   {
      boost::mutex::scoped_lock lock(m_mutex);

      if (observer)
         m_observers.push_back(observer);
      else
         throw shared::exception::CNullReference("Observer can not be null");
   }


   void CNotificationCenter::unsubscribeObserver(boost::shared_ptr< IObserver > observer)
   {
      boost::mutex::scoped_lock lock(m_mutex);

      if (observer)
      {
         std::vector< boost::shared_ptr< IObserver > >::iterator i;
         for (i = m_observers.begin(); i != m_observers.end(); ++i)
         {
            if (*i == observer)
            {
               m_observers.erase(i);
               return;
            }
         }
      }
      else
      {
         throw shared::exception::CNullReference("Observer can not be null");
      }
   }



} // namespace notification