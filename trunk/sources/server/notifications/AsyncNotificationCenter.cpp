#include "stdafx.h"
#include "AsyncNotificationCenter.h"
#include <Poco/ActiveMethod.h>

namespace notifications {

   //-----------------------------
   ///\brief Declare the static internal notification center
   //-----------------------------
   Poco::SharedPtr<CAsyncNotificationCenter> CAsyncNotificationCenter::m_instance;
   Poco::FastMutex CAsyncNotificationCenter::m_instanceMutex;

   
   CAsyncNotificationCenter* CAsyncNotificationCenter::get()
   {
      Poco::FastMutex::ScopedLock lock(m_instanceMutex);
      if (!m_instance) m_instance.assign(new CAsyncNotificationCenter);
      return m_instance.get();
   }
   
   void CAsyncNotificationCenter::postNotification(const Poco::Notification::Ptr & pNotification)
   {
      m_center.postNotification(pNotification);
   }

   void CAsyncNotificationCenter::postNotificationAsync(Poco::Notification::Ptr pNotification)
   {
      Poco::ActiveMethod<void, Poco::Notification::Ptr, CAsyncNotificationCenter> ayncPostNotification(this, &CAsyncNotificationCenter::postNotification);
      ayncPostNotification(pNotification);
   }


   void CAsyncNotificationCenter::addObserver(const Poco::AbstractObserver& observer)
   {
      m_center.addObserver(observer);
   }

   void CAsyncNotificationCenter::removeObserver(const Poco::AbstractObserver& observer)
   {
      m_center.removeObserver(observer);
   }

   bool CAsyncNotificationCenter::hasObservers()
   {
      return m_center.hasObservers();
   }

   std::size_t CAsyncNotificationCenter::countObservers()
   {
      return m_center.countObservers();
   }

} //namespace notifications 
