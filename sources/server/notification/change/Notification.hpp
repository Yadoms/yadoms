#pragma once

#include "../basic/Notification.hpp"
#include "Type.h"

namespace notification {   namespace change {
   //-----------------------------
   ///\brief Notification for CUD (Create/Update/Delete on object)
   ///\template T The notification content type
   //-----------------------------
   template<class T>
   class CNotification : public basic::CNotification< T >
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in] obj   The notification object
      ///\param [in] changeType   The type of CUD (create, update or delete) which affect the object
      //-----------------------------
      CNotification(boost::shared_ptr< T > obj, EChangeType changeType)
         :basic::CNotification< T >(obj), m_changeType(changeType)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNotification()
      {

      }

      //-----------------------------
      ///\brief Get the CUD type
      ///\return the CUD type
      //-----------------------------
      EChangeType getChangeType()
      {
         return m_changeType;
      }

   private:
      //-----------------------------
      ///\brief The CUD type
      //-----------------------------
      EChangeType m_changeType;
   };

}
}

