#pragma once
#include <Poco/Notification.h>
#include <shared/Log.h>
#include "database/entities/Entities.h"
namespace notifications {

   //-----------------------------
   ///\brief Notification for a new acquisition
   //-----------------------------
   class CNewDeviceNotification : public Poco::Notification
   {
   public:

      //-----------------------------
      ///\brief Constructor
      ///\param [in]  device   The new device
      CNewDeviceNotification(boost::shared_ptr< database::entities::CDevice > device)
         :m_device(device)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNewDeviceNotification()
      {
      }

      //-----------------------------
      ///\brief Get the new device
      ///\return  the new device
      //-----------------------------
      boost::shared_ptr< database::entities::CDevice > getDevice() const { return m_device; }

   private:
      //-----------------------------
      ///\brief The new device
      //-----------------------------
      boost::shared_ptr< database::entities::CDevice > m_device;
   };

} //namespace notifications 
