#pragma once

#include "../change/Notification.hpp"
#include "database/entities/Entities.h"

namespace notification
{
   namespace acquisition
   {
      //-----------------------------
      ///\brief Interface for basic notification (handle a typed object)
      //-----------------------------
      class CNotification final : public change::CNotification<database::entities::CAcquisition>
      {
      public:
         explicit CNotification(const boost::shared_ptr<database::entities::CAcquisition>& acquisition)
            : change::CNotification<database::entities::CAcquisition>(acquisition,
                                                                      change::EChangeType::kCreate)
         {
         }

         ~CNotification() override = default;

         boost::shared_ptr<database::entities::CAcquisition> getAcquisition()
         {
            return getObject();
         }
      };
   } //namespace acquisition
} //namespace notification
