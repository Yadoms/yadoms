#pragma once
#include "Poco/NotificationCenter.h"

namespace notifications {

   //-----------------------------
   ///\brief Notification center
   //-----------------------------
   class CAsyncNotificationCenter
   {
   public:
      //-----------------------------
      ///\brief Post a notification (blocking method : wait until all observers have been called).
      ///\param [in] pNotification  The notification
      //-----------------------------
      void postNotification(const Poco::Notification::Ptr & pNotification);

      //-----------------------------
      ///\brief Post a notification (non blocking method : returns immediatetly).
      ///\param [in] pNotification  The notification
      //-----------------------------
      void postNotificationAsync(Poco::Notification::Ptr pNotification);

      //-----------------------------
      ///\brief Add an observer
      ///\param [in] observer  The observer to add
      //-----------------------------
      void addObserver(const Poco::AbstractObserver& observer);

      //-----------------------------
      ///\brief Remove an observer
      ///\param [in] observer  The observer to remlove
      //-----------------------------
      void removeObserver(const Poco::AbstractObserver& observer);

      //-----------------------------
      ///\brief Check at least one observer is registered
      ///\return true if at least one observer is registered; overwise false
      //-----------------------------
      bool hasObservers();

      //-----------------------------
      ///\brief Get the observer count
      ///\return the number of registered observers
      //-----------------------------
      std::size_t countObservers();
   
      //-----------------------------
      ///\brief Get the unique instance of CAsyncNotificationCenter
      ///\return a pointer to CAsyncNotificationCenter
      //-----------------------------
      static CAsyncNotificationCenter* get();

   private:
      //-----------------------------
      ///\brief Constructor is private; can access only by get
      //-----------------------------
      CAsyncNotificationCenter()
      {
      }

      //-----------------------------
      ///\brief internal static Notification center
      //-----------------------------
      Poco::NotificationCenter m_center;

      //-----------------------------
      ///\brief Pointer (auto) on AyncNotificationCenter
      //-----------------------------
      static Poco::SharedPtr<CAsyncNotificationCenter> m_instance;
      //-----------------------------
      ///\brief Mutex to protect access of m_instance
      //-----------------------------
      static Poco::FastMutex m_instanceMutex;
   };

} //namespace notifications 
