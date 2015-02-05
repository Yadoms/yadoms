#include "stdafx.h"
#include "NotificationObserver.h"

namespace shared { namespace notification{

   CNotificationObserver::CNotificationObserver()
      :m_isWaiting(false)
   {
   }

   CNotificationObserver::~CNotificationObserver()
   {
   }

   int CNotificationObserver::waitForNotifications(const boost::posix_time::time_duration& timeout)
   {
      m_isWaiting = true;
      try
      {
         int result = m_eventHandler.waitForEvents(timeout);
         m_isWaiting = false;
         return result;
      }
      catch (...)
      {
         m_isWaiting = false;
         throw;
      }
   }

   bool CNotificationObserver::isWaiting() const
   {
      return m_isWaiting;
   }
} //namespace notification
} //namespace shared
