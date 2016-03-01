#pragma once

#include "../INotification.h"
#include "../change/Notification.hpp"

namespace notification { namespace acquisition {
   
   //-----------------------------
   ///\brief Interface for basic notification (handle a typed object)
   //-----------------------------
   class CNotification : public change::CNotification<database::entities::CAcquisition>
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in] obj   The notification object
      //-----------------------------
      explicit CNotification(boost::shared_ptr<database::entities::CAcquisition> acquisition)
         :change::CNotification<database::entities::CAcquisition>(acquisition, notification::change::EChangeType::kCreate)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNotification()
      {
      }

      //-----------------------------
      ///\brief Get the acquisition
      ///\return The acquisition
      //-----------------------------
      boost::shared_ptr<database::entities::CAcquisition> getAcquisition()
      {
         return getObject();
      }
   };

} //namespace acquisition
} //namespace notification