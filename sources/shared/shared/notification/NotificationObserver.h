#pragma once
#include <shared/Export.h>
#include <shared/event/EventHandler.hpp>

namespace shared { namespace notification{

   class YADOMS_SHARED_EXPORT CNotificationObserver
   {
   public:
      CNotificationObserver();
      virtual ~CNotificationObserver();

      template<class T>
      void postNotification(int typeOfNotif, const T & notif)
      {
         m_EventHandler->postEvent(typeOfNotif, notif);
      }

      template<class T>
      T getNotificationData()
      {
         return m_EventHandler->getEventData<T>();
      }

      template<class T>
      bool isNotificationTypeOf()
      {
         return m_EventHandler->isEventType<T>();
      }

      int waitForNotifications(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

      bool isWaiting() const;
   private:
      //-----------------------------------------------------------------------------
      /// \brief		                     The event handler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_EventHandler;

      //-----------------------------------------------------------------------------
      /// \brief		                     Indicate if this observer is already waiting for event
      //-----------------------------------------------------------------------------
      bool m_isWaiting;
   };


} //namespace notification
} //namespace shared
