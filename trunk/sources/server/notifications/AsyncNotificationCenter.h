#pragma once
#include <shared/notification/NotificationCenter.h>

namespace notifications {

   //-----------------------------
   ///\brief Notification center
   //-----------------------------
   class CAsyncNotificationCenter
   {
   public:
      //-----------------------------
      ///\brief Get the unique instance of CNotificationCenter
      ///\return a pointer to CNotificationCenter
      //-----------------------------
      static shared::notification::CNotificationCenter* get();

   private:

      //-----------------------------
      ///\brief Pointer (auto) on AyncNotificationCenter
      //-----------------------------
      static boost::shared_ptr<shared::notification::CNotificationCenter> m_instance;
      //-----------------------------
      ///\brief Mutex to protect access of m_instance
      //-----------------------------
      static boost::mutex m_instanceMutex;
   };

} //namespace notifications 
