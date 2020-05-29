#pragma once

#include <shared/ServiceLocator.h>
#include "NotificationCenter.h"
#include "IObserver.h"

#include "basic/Notification.hpp"
#include "basic/Observer.hpp"

#include "change/Notification.hpp"
#include "change/Observer.hpp"
#include "change/Type.h"

#include "action/IAction.h"
#include "action/FunctionPointerAction.hpp"
#include "action/EventAction.hpp"

#include <shared/event/EventHandler.hpp>

namespace notification
{
   //-----------------------------
   ///\brief Helper class for notification center usage
   /// The aim of this class is to provide methods which allow really easy use of notification center
   ///
   /// Sample usage 1 : post an object (can be anything) If the object do not implements INotification, then the object is encapsulated into a basic::CNotification
   ///
   ///      //create 
   ///      boost::shared_ptr<CMyClass> myObject(boost::make_shared<CMyClass>(...));
   ///      //post notification
   ///      notification::CHelpers::postNotification(myObject);
   ///
   /// Sample usage 2 : post a new object (device) creation
   ///
   ///      //create the object (device)
   ///      boost::shared_ptr<database::entities::CDevice> newDevice = ....;
   ///      //post notification
   ///      notification::CHelpers::postChangeNotification(newDevice, notification::change::EChangeType::kCreate);
   ///
   ///
   ///
   /// Sample usage 3 : subscribe for my object notification with EventHandler callback (match sample usage 1)
   ///
   ///      // Subscribe to CMyClass notifications
   ///      observer = notification::CHelpers::subscribeBasicObserver< CMyClass >(eventHandler, kMyObjectNotification);
   ///
   /// Sample usage 4 : subscribe for my object notification with function pointer callback (match sample usage 1)
   ///
   ///      void COtherClass::onCMyClassNotif(boost::shared_ptr<CMyClass> object) { //do something }
   ///
   ///      // Subscribe to new device notifications
   ///      observer = notification::CHelpers::subscribeBasicObserver< CMyClass >(boost::bind(&COtherClass::onCMyClassNotif, this, boost::placeholders::_1) );
   ///
   /// Sample usage 5 : subscribe for new device creation notification with EventHandler callback (match sample usage 2)
   ///
   ///      // Subscribe to new device notifications
   ///      observer = notification::CHelpers::subscribeChangeObserver< database::entities::CDevice >(notification::change::EChangeType::kCreate, eventHandler, kNewDevice);
   ///
   /// Sample usage 6 : subscribe for new device creation notification with function pointer callback (match sample usage 2)
   ///
   ///      void CMyClass::onNewDevice(boost::shared_ptr<database::entities::CDevice> newDevice) { //do something }
   ///
   ///      // Subscribe to new device notifications
   ///      observer = notification::CHelpers::subscribeChangeObserver< database::entities::CDevice >(notification::change::EChangeType::kCreate, boost::bind(&CMyClass::onNewDevice, this, boost::placeholders::_1) );
   ///
   /// Sample usage 7 : unsubscribe observer (matching sample 3, 4, 5 and 6)
   ///
   ///      //DO always remove observers
   ///      notification::CHelpers::unsubscribeObserver(observer);
   //-----------------------------
   class CHelpers
   {
   public:
      //-----------------------------
      ///\brief Post a notification
      ///      Manage two cases:
      ///         1. if the parameter is an implementation of INotification, just post the notification
      ///         1. if the parameter is NOT an implementation of INotification, encapsulate the object into a basic notification and post the notification
      ///\param [in] n                    The notification to post (shared_ptr)
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N  The notification data type
      //-----------------------------
      template <class N>
      static void postNotification(const boost::shared_ptr<N> n, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         if (n)
         {
            //if notification center is not provided, get the global scope service locator instance
            if (!notificationCenter)
            {
               notificationCenter = shared::CServiceLocator::instance().get<CNotificationCenter>();

               //check
               if (!notificationCenter)
                  throw shared::exception::CNullReference("Notification center not found");
            }

            boost::shared_ptr<INotification> compatibleNotification = boost::dynamic_pointer_cast<INotification>(n);

            //if notification object do not implements INotification,
            //then create a basic notification
            if (!compatibleNotification)
            {
               compatibleNotification = boost::make_shared<basic::CNotification<N>>(n);
            }

            if (compatibleNotification)
            {
               notificationCenter->postNotification(compatibleNotification);
            }
            else
            {
               throw shared::exception::CNullReference("Incompatible notification data");
            }
         }
         else
         {
            throw shared::exception::CNullReference("Notification can not be null");
         }
      }


      //-----------------------------
      ///\brief Post a notification
      ///      Manage two cases:
      ///         1. if the parameter is an implementation of INotification, just post the notification
      ///         1. if the parameter is NOT an implementation of INotification, encapsulate the object into a basic notification and post the notification
      ///\param [in] n                    The notification to post (shared_ptr)
      ///\param [in] changeType           The type of change notification to observer (create, update or delete)
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                      The notification data type
      ///\return The created observer
      //-----------------------------
      template <class N>
      static void postChangeNotification(const boost::shared_ptr<N> n, change::EChangeType changeType, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         if (n)
         {
            //if notification center is not provided, get the global scope service locator instance
            if (!notificationCenter)
            {
               notificationCenter = shared::CServiceLocator::instance().get<CNotificationCenter>();

               //check
               if (!notificationCenter)
                  throw shared::exception::CNullReference("Notification center not found");
            }

            boost::shared_ptr<INotification> compatibleNotification(boost::make_shared<change::CNotification<N>>(n, changeType));
            notificationCenter->postNotification(compatibleNotification);
         }
         else
         {
            throw shared::exception::CNullReference("Notification can not be null");
         }
      }


      //-----------------------------
      ///\brief Subscribe a basic observer, which will call a function pointer callback
      ///\param [in] functor              The function pointer to callback when a basic notification matching N is posted
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                      The notification type to observe
      ///\return The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeBasicObserver(typename action::CFunctionPointerNotifier<N>::FunctionPtr functor, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         if (functor)
         {
            //create the action
            boost::shared_ptr<action::IAction<N>> observerAction(boost::make_shared<action::CFunctionPointerNotifier<N>>(functor));
            return subscribeBasicObserver<N>(observerAction, notificationCenter);
         }
         else
         {
            throw shared::exception::CNullReference("Functor can not be null");
         }
      }

      //-----------------------------
      ///\brief Subscribe an observer, which will post an event 
      ///\param [in] eventHandler            The eventHandler handler
      ///\param [in] notificationEventCode   The notification code
      ///\param [in] notificationCenter      The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                         The notification type to observe
      ///\return The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeBasicObserver(shared::event::CEventHandler& eventHandler, const int notificationEventCode, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //create the action
         boost::shared_ptr<action::IAction<N>> observerAction(boost::make_shared<action::CEventAction<N>>(eventHandler, notificationEventCode));
         return subscribeBasicObserver<N>(observerAction, notificationCenter);
      }

      //-----------------------------
      ///\brief Subscribe an observer, which will post an event 
      ///\param [in] eventHandler            The eventHandler handler (shared_ptr)
      ///\param [in] notificationEventCode   The notification code
      ///\param [in] notificationCenter      The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                         The notification type to observe
      ///\return The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeBasicObserver(boost::shared_ptr<shared::event::CEventHandler> eventHandler, const int notificationEventCode, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //create the action
         boost::shared_ptr<action::IAction<N>> observerAction(boost::make_shared<action::CEventPtrAction<N>>(eventHandler, notificationEventCode));
         return subscribeBasicObserver<N>(observerAction, notificationCenter);
      }


      //-----------------------------
      ///\brief Subscribe a basic observer, which will call a function pointer callback
      ///\param [in] changeType           The type of change notification to observer (create, update or delete)
      ///\param [in] functor              The function pointer to callback when a basic notification matching N is posted
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                      The notification type to observe
      ///\return The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeChangeObserver(change::EChangeType changeType, typename action::CFunctionPointerNotifier<N>::FunctionPtr functor, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         if (functor)
         {
            //create the action
            boost::shared_ptr<action::IAction<N>> observerAction(boost::make_shared<action::CFunctionPointerNotifier<N>>(functor));

            return subscribeChangeObserver<N>(changeType, observerAction, notificationCenter);
         }

         throw shared::exception::CNullReference("Functor can not be null");
      }

      //-----------------------------
      ///\brief Subscribe a change observer, which will post an event 
      ///\param [in] changeType              The type of change notification to observer (create, update or delete)
      ///\param [in] eventHandler            The eventHandler handler
      ///\param [in] notificationEventCode   The notification code
      ///\param [in] notificationCenter      The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                         The notification type to observe
      ///\return  The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeChangeObserver(change::EChangeType changeType, shared::event::CEventHandler& eventHandler, const int notificationEventCode, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //create the action
         boost::shared_ptr<action::IAction<N>> observerAction(boost::make_shared<action::CEventAction<N>>(eventHandler, notificationEventCode));
         return subscribeChangeObserver<N>(changeType, observerAction, notificationCenter);
      }

      //-----------------------------
      ///\brief Subscribe a change observer, which will post an event 
      ///\param [in] changeType              The type of change notification to observer (create, update or delete)
      ///\param [in] eventHandler            The eventHandler handler (shared_ptr)
      ///\param [in] notificationEventCode   The notification code
      ///\param [in] notificationCenter      The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                         The notification type to observe
      ///\return  The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeChangeObserver(change::EChangeType changeType, boost::shared_ptr<shared::event::CEventHandler> eventHandler, const int notificationEventCode, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //create the action
         boost::shared_ptr<action::IAction<N>> observerAction(boost::make_shared<action::CEventPtrAction<N>>(eventHandler, notificationEventCode));
         return subscribeChangeObserver<N>(changeType, observerAction, notificationCenter);
      }


      //-----------------------------
      ///\brief unsubscribe an observer
      ///\param [in] obs                  The observer to unregister
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      //-----------------------------
      static void unsubscribeObserver(boost::shared_ptr<IObserver> observer, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         if (observer)
         {
            //if notification center is not provided, get the global scope service locator instance
            if (!notificationCenter)
            {
               notificationCenter = shared::CServiceLocator::instance().get<CNotificationCenter>();

               //check
               if (!notificationCenter)
                  throw shared::exception::CNullReference("Notification center not found");
            }

            notificationCenter->unsubscribeObserver(observer);
         }
         else
         {
            throw shared::exception::CNullReference("Observer can not be null");
         }
      }


      //-----------------------------
      ///\brief Subscribe a custom observer, which will call a function pointer callback
      ///\param [in] functor              The function pointer to callback when a basic notification matching N is posted
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                      The notification type to observe
      ///\return The created observer
      //-----------------------------
      static void subscribeCustomObserver(boost::shared_ptr<IObserver> observer, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //if notification center is not provided, get the global scope service locator instance
         if (!notificationCenter)
         {
            notificationCenter = shared::CServiceLocator::instance().get<CNotificationCenter>();

            //check
            if (!notificationCenter)
               throw shared::exception::CNullReference("Notification center not found");
         }

         notificationCenter->subscribeObserver(observer);
      }

      //-----------------------------
      ///\brief Helper class to subscribe and auto-unsubscribe a custom observer
      //-----------------------------
      class CCustomSubscriber
      {
      public:
         //-----------------------------
         ///\brief                           Constructor
         ///\param [in] observer             The observer to subscribe
         ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
         //-----------------------------
         CCustomSubscriber(boost::shared_ptr<IObserver> observer, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
            : m_observer(observer), m_notificationCenter(notificationCenter)
         {
            subscribeCustomObserver(m_observer, m_notificationCenter);
         }

         //-----------------------------
         ///\brief                           Destructor
         //-----------------------------
         virtual ~CCustomSubscriber()
         {
            unsubscribeObserver(m_observer, m_notificationCenter);
         }

      private:
         boost::shared_ptr<IObserver> m_observer;
         boost::shared_ptr<CNotificationCenter> m_notificationCenter;
      };

   private:
      //-----------------------------
      ///\brief Subscribe a basic observer
      ///\param [in] action               The action to execute when notification is posted
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                      The notification type to observe
      ///\return The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeBasicObserver(boost::shared_ptr<action::IAction<N>> action, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //if notification center is not provided, get the global scope service locator instance
         if (!notificationCenter)
         {
            notificationCenter = shared::CServiceLocator::instance().get<CNotificationCenter>();

            //check
            if (!notificationCenter)
               throw shared::exception::CNullReference("Notification center not found");
         }

         //create a basic observer
         boost::shared_ptr<IObserver> basicObserver(boost::make_shared<basic::CObserver<N>>(action));

         notificationCenter->subscribeObserver(basicObserver);

         return basicObserver;
      }

      //-----------------------------
      ///\brief Subscribe a change observer
      ///\param [in] changeType           The type of change to observe
      ///\param [in] action               The action to execute when notification is posted
      ///\param [in] notificationCenter   The notification center (if not specified, it tries to get the service located NotificationCenter)
      ///\template N                      The notification type to observe
      ///\return The created observer
      //-----------------------------
      template <class N>
      static boost::shared_ptr<IObserver> subscribeChangeObserver(change::EChangeType changeType, boost::shared_ptr<action::IAction<N>> action, boost::shared_ptr<CNotificationCenter> notificationCenter = boost::shared_ptr<CNotificationCenter>())
      {
         //if notification center is not provided, get the global scope service locator instance
         if (!notificationCenter)
         {
            notificationCenter = shared::CServiceLocator::instance().get<CNotificationCenter>();

            //check
            if (!notificationCenter)
               throw shared::exception::CNullReference("Notification center not found");
         }

         //create a basic observer
         boost::shared_ptr<IObserver> changeObserver(boost::make_shared<change::CObserver<N>>(changeType, action));
         notificationCenter->subscribeObserver(changeObserver);
         return changeObserver;
      }
   };
} // namespace notification


