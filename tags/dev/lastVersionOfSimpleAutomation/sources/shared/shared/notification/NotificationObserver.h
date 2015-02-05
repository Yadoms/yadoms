#pragma once
#include <shared/Export.h>
#include <shared/event/EventHandler.hpp>

namespace shared { namespace notification{

   class YADOMS_SHARED_EXPORT CNotificationObserver
   {
   public:
      CNotificationObserver();
      virtual ~CNotificationObserver();

   private:
      // Avoid copy
      CNotificationObserver(const CNotificationObserver&);
      const CNotificationObserver& operator=(const CNotificationObserver&);

   public:

      template<class T>
      void postNotification(int typeOfNotif, const T & notif)
      {
         m_eventHandler.postEvent<T>(typeOfNotif, notif);
      }

      template<class T>
      T getNotificationData()
      {
         return m_eventHandler.getEventData<T>();
      }

      template<class T>
      bool isNotificationTypeOf()
      {
         return m_eventHandler.isEventType<T>();
      }

      int waitForNotifications(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

      bool isWaiting() const;
   private:
      //-----------------------------------------------------------------------------
      /// \brief		                     The event handler
      //-----------------------------------------------------------------------------
      shared::event::CEventHandler m_eventHandler;

      //-----------------------------------------------------------------------------
      /// \brief		                     Indicate if this observer is already waiting for event
      //-----------------------------------------------------------------------------
      bool m_isWaiting;
   };


} //namespace notification
} //namespace shared
