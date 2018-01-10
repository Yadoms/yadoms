#pragma once

#include "IObserver.h"
#include "INotification.h"

namespace notification {

   //-----------------------------
   ///\brief Notification center
   //-----------------------------
   class CNotificationCenter
   {
   public:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------
      CNotificationCenter();

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNotificationCenter();

      //-----------------------------
      ///\brief Post a notification
      ///      Manage two cases:
      ///         1. if the parameter is an implementation of INotification, just post the notification
      ///         1. if the parameter is NOT an implementation of INotification, encapsulate the object into a basic notification and post the notification
      ///\param [in] notification  The notification to post (shared_ptr)
      //-----------------------------
      void postNotification(const boost::shared_ptr<INotification> notification);

      //-----------------------------
      ///\brief Subscribe an observer
      ///\param [in] observer  The observer to register
      //-----------------------------
      void subscribeObserver(boost::shared_ptr< IObserver > observer);

      //-----------------------------
      ///\brief Unsubscribe an observer
      ///\param [in] observer  The observer to unregister
      //-----------------------------
      void unsubscribeObserver(boost::shared_ptr< IObserver > observer);
   
   private:
      //-----------------------------
      ///\brief The observers container
      //-----------------------------
      std::vector< boost::shared_ptr< IObserver > > m_observers;

      //-----------------------------
      ///\brief The observers container mutex
      //-----------------------------
      mutable boost::mutex m_mutex;
   };

} // namespace notification