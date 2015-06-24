#pragma once

#include "Notification.hpp"

namespace notification { namespace basic {
   
   //-----------------------------
   ///\brief Interface for basic notification (handle a typed object)
   ///\template T The notification content type
   //-----------------------------
   template<class T>
   class CNotification : public INotification
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in] obj   The notification object
      //-----------------------------
      CNotification(boost::shared_ptr<T> obj)
         :m_obj(obj)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNotification()
      {
      }

      //-----------------------------
      ///\brief Get the notification object
      ///\return The notification object
      //-----------------------------
      boost::shared_ptr<T> getObject()
      {
         return m_obj;
      }

   private:
      //-----------------------------
      ///\brief The notification object
      //-----------------------------
      boost::shared_ptr<T> m_obj;
   };

} //namespace basic
} //namespace notification