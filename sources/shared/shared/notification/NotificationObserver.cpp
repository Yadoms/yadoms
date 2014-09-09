#include "stdafx.h"
#include "NotificationObserver.h"

namespace shared { namespace notification{

   CNotificationObserver::CNotificationObserver()
      :m_EventHandler(new shared::event::CEventHandler())
   {
   }

   CNotificationObserver::~CNotificationObserver()
   {

   }

   int CNotificationObserver::waitForNotifications(const boost::posix_time::time_duration& timeout)
   {
      return m_EventHandler->waitForEvents(timeout);
   }

} //namespace notification
} //namespace shared
