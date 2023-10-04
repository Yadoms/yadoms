#include "stdafx.h"
#include "NotificationCenter.h"
#include <shared/exception/NullReference.hpp>

namespace notification
{
   CNotificationCenter::CNotificationCenter()
   {
   }

   CNotificationCenter::~CNotificationCenter()
   {
   }

   void CNotificationCenter::postNotification(const boost::shared_ptr<INotification>& notification) const
   {
      if (notification)
      {
         //make the observers list copy, and release mutex
         boost::mutex::scoped_lock lock(m_mutex);
         const std::vector<boost::shared_ptr<IObserver>> localList(m_observers);
         lock.unlock();

         for (const auto& i : localList)
            i->observe(notification);
      }
      else
      {
         throw shared::exception::CNullReference("Notification can not be null");
      }
   }


   void CNotificationCenter::subscribeObserver(const boost::shared_ptr<IObserver>& observer)
   {
      boost::mutex::scoped_lock lock(m_mutex);

      if (observer)
         m_observers.push_back(observer);
      else
         throw shared::exception::CNullReference("Observer can not be null");
   }


   void CNotificationCenter::unsubscribeObserver(const boost::shared_ptr<IObserver>& observer)
   {
      boost::mutex::scoped_lock lock(m_mutex);

      if (observer)
      {
         for (auto observerIterator = m_observers.begin(); observerIterator != m_observers.end(); ++observerIterator)
         {
            if (*observerIterator == observer)
            {
               m_observers.erase(observerIterator);
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
