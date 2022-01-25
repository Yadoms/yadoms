#pragma once
#include "INewAcquisitionObserver.h"

namespace dataAccessLayer
{
   namespace observers
   {
      class IAcquisitionObservers
      {
      public:
         virtual ~IAcquisitionObservers() = default;

         virtual void addObserver(boost::shared_ptr<INewAcquisitionObserver> observer) = 0;
         virtual void removeObserver(boost::shared_ptr<INewAcquisitionObserver> observer) = 0;
         virtual void onNewAcquisition(const boost::shared_ptr<database::entities::CAcquisition>& acquisition) = 0;
      };
   } //namespace observers
} //namespace dataAccessLayer 
