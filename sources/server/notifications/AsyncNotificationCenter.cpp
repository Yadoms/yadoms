#include "stdafx.h"
#include "AsyncNotificationCenter.h"

namespace notifications {

   //-----------------------------
   ///\brief Declare the static internal notification center
   //-----------------------------
   boost::shared_ptr<shared::notification::CNotificationCenter> CAsyncNotificationCenter::m_instance;
   boost::mutex CAsyncNotificationCenter::m_instanceMutex;
   
   shared::notification::CNotificationCenter* CAsyncNotificationCenter::get()
   {
      boost::mutex::scoped_lock scoped_lock(m_instanceMutex);
      if (!m_instance) m_instance.reset(new shared::notification::CNotificationCenter());
      return m_instance.get();
   }
   

} //namespace notifications 
