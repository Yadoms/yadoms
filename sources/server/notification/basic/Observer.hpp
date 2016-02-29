#pragma once

#include "../IObserver.h"
#include "../action/IAction.h"
#include "Notification.hpp"

namespace notification { namespace basic {
      
   //-----------------------------
   ///\brief Basic observer, just expect a matching notification type
   ///\template T The notification content type
   //-----------------------------
   template<class T>
   class CObserver : public IObserver
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in] action The action (what to do when the good notification appears)
      //-----------------------------
      explicit CObserver(boost::shared_ptr< action::IAction<T> > action)
         :m_action(action)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CObserver() {
      }

      //IObserver implementation
      void observe(const boost::shared_ptr< INotification > n)
      {
         //check notification is good type
         boost::shared_ptr< CNotification<T> > notif = boost::dynamic_pointer_cast< CNotification<T> >(n);
         if (notif && m_action)
         {
            //do notification
            m_action->sendNotification(notif->getObject());
         }
      }
      // [END] - IObserver implementation

   private:
      //-----------------------------
      ///\brief The action to process when notification is posted
      //-----------------------------
      boost::shared_ptr< action::IAction<T> > m_action;
   };


} // namespace observer
} // namespace notification
