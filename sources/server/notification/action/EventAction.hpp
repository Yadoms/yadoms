#pragma once

#include "IAction.h"
#include <shared/event/EventHandler.hpp>

namespace notification
{
   namespace action
   {
      //-----------------------------
      ///\brief Provide a notification action using event handler
      ///\template T The notification content type
      //-----------------------------
      template <class T>
      class CEventAction : public IAction<T>
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param [in] eventHandler      The event handler to use
         ///\param [in] notificationCode  The event handler code to use
         //-----------------------------
         CEventAction(shared::event::CEventHandler& eventHandler,
                      const int notificationCode)
            : m_eventHandler(eventHandler),
              m_notificationCode(notificationCode)
         {
         }

         ~CEventAction() override = default;

         //IAction<T> implementation
         void sendNotification(boost::shared_ptr<T> notification) override
         {
            m_eventHandler.postEvent(m_notificationCode,
                                     notification);
         }

         // [END] - IAction<T> implementation

      private:
         //-----------------------------
         ///\brief The event handler to use
         //-----------------------------
         shared::event::CEventHandler& m_eventHandler;

         //-----------------------------
         ///\brief The event handler code to use
         //-----------------------------
         int m_notificationCode;
      };


      //-----------------------------
      ///\brief Provide a notification action using event handler (as shared_ptr)
      ///\template T The notification content type
      //-----------------------------
      template <class T>
      class CEventPtrAction : public IAction<T>
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param [in] eventHandler      The event handler to use
         ///\param [in] notificationCode  The event handler code to use
         //-----------------------------
         CEventPtrAction(const boost::shared_ptr<shared::event::CEventHandler>& eventHandler,
                         const int notificationCode)
            : m_eventHandler(eventHandler),
              m_notificationCode(notificationCode)
         {
         }

         ~CEventPtrAction() override = default;

         //IAction<T> implementation
         void sendNotification(boost::shared_ptr<T> notification) override
         {
            m_eventHandler->postEvent(m_notificationCode,
                                      notification);
         }

         // [END] - IAction<T> implementation

      private:
         //-----------------------------
         ///\brief The event handler to use
         //-----------------------------
         boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;

         //-----------------------------
         ///\brief The event handler code to use
         //-----------------------------
         int m_notificationCode;
      };

      //-----------------------------
      ///\brief Provide a notification action using functor
      ///\template T The notification content type
      //-----------------------------
      template <class T>
      class CFunctorEventAction : public IAction<T> //TODO utile ?
      {
      public:
         CFunctorEventAction(const std::function<void(boost::shared_ptr<T> notification)>& onEventFct)
            : m_onEventFct(onEventFct)
         {
         }

         ~CFunctorEventAction() override = default;

         //IAction<T> implementation
         void sendNotification(boost::shared_ptr<T> notification) override
         {
            m_onEventFct(notification);
         }

         // [END] - IAction<T> implementation

      private:
         std::function<void(boost::shared_ptr<T> notification)> m_onEventFct;
      };
   } // namespace action
} // namespace notification
