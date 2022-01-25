#pragma once
#include "database/entities/Entities.h"

namespace dataAccessLayer
{
   namespace observers
   {
      class INewAcquisitionObserver
      {
      public:
         virtual ~INewAcquisitionObserver() = default;

         virtual void onNewAcquisition(boost::shared_ptr<database::entities::CAcquisition> acquisition) = 0;
      };
   } //namespace observers
} //namespace dataAccessLayer 
