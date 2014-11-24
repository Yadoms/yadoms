#include "stdafx.h"
#include "NotificationObserver.h"

namespace shared { namespace notification{

   CNotificationObserver::CNotificationObserver()
   {
   }

   CNotificationObserver::~CNotificationObserver()
   {
   }

   int CNotificationObserver::waitForNotifications(const boost::posix_time::time_duration& timeout, const int pollingEvent)
   {
      // Use here a timer instead of timeout, as workarround on a rarely crash in CEventHandler only when used by CNotificationObserver (!?!?)
      // TODO De toutes façons, c'est pas bien clean de faire du polling pour savoir si on a quelque chose sur la websocket, ne pourrait-on
      // pas être notifié sur réception ? Be ça bouffe des ressources...
      m_eventHandler.createTimer(pollingEvent, shared::event::CEventTimer::kOneShot, timeout);
      int result = m_eventHandler.waitForEvents();
      return result;
   }
} //namespace notification
} //namespace shared
