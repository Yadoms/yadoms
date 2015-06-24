#pragma once

#include "../IObserver.h"
#include "../action/IAction.h"
#include "Notification.hpp"
#include "Type.h"

namespace notification { namespace change {
      
   
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
      ///\param [in] changeType  The CUD type to observer
      ///\param [in] action      The action (what to do when the good notification appears)
      //-----------------------------
      CObserver(EChangeType changeType, boost::shared_ptr< action::IAction<T> > action)
         :m_changeType(changeType), m_action(action)
      {

      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CObserver()
      {
      }

      //IObserver implementation
      void observe(const boost::shared_ptr< INotification > n)
      {
         //check notification is good type
         boost::shared_ptr< CNotification<T> > notif = boost::dynamic_pointer_cast< CNotification< T > >(n);
         if (notif && notif->getChangeType() == m_changeType && m_action)
         {
            //do notification
            m_action->sendNotification(notif->getObject());
         }
      }
      // [END] - IObserver implementation

   protected:
      //-----------------------------
      ///\brief Type of change applied to notification object (CUD)
      //-----------------------------
      EChangeType m_changeType;

      //-----------------------------
      ///\brief The action to process when [notification and changeType] is posted
      //-----------------------------
      boost::shared_ptr< action::IAction<T> > m_action;
   };


} // namespace change
} // namespace notification
