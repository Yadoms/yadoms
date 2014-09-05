#pragma once
#include <Poco/Notification.h>
#include <shared/Log.h>
#include "database/entities/Entities.h"
namespace notifications {

   //-----------------------------
   ///\brief Notification for a new acquisition
   //-----------------------------
   class CNewAcquisitionNotification : public Poco::Notification
   {
   public:

      //-----------------------------
      ///\brief Constructor
      ///\param [in]  keywordId   The keyword id which have a new data
      ///\param [in]  value       The value
      //-----------------------------
      CNewAcquisitionNotification(boost::shared_ptr< database::entities::CAcquisition > acquisition)
         :m_acquisition(acquisition)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNewAcquisitionNotification()
      {
      }

      //-----------------------------
      ///\brief Get the keyword id
      ///\return  the keyword id
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisition > getAcquisition() const { return m_acquisition; }

   private:
      //-----------------------------
      ///\brief The keyword id
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisition > m_acquisition;
   };

} //namespace notifications 
