#pragma once

#include "IObserver.h"
#include "INotification.h"

namespace notification
{
   //-----------------------------
   ///\brief Notification center
   //-----------------------------
   class CNotificationCenter final
   {
   public:
      CNotificationCenter();
      ~CNotificationCenter();

      //-----------------------------
      ///\brief Post a notification
      ///\param [in] notification  The notification to post
      //-----------------------------
      void postNotification(const boost::shared_ptr<INotification>& notification) const;

      //-----------------------------
      ///\brief Subscribe an observer
      ///\param [in] observer  The observer to register
      //-----------------------------
      void subscribeObserver(const boost::shared_ptr<IObserver>& observer);

      //-----------------------------
      ///\brief Unsubscribe an observer
      ///\param [in] observer  The observer to unregister
      //-----------------------------
      void unsubscribeObserver(const boost::shared_ptr<IObserver>& observer);

   private:
      //-----------------------------
      ///\brief The observers container
      //-----------------------------
      std::vector<boost::shared_ptr<IObserver>> m_observers;

      //-----------------------------
      ///\brief The observers container mutex
      //-----------------------------
      mutable boost::mutex m_mutex;
   };
} // namespace notification
