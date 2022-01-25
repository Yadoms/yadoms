#pragma once
#include "IAcquisitionObservers.h"

namespace dataAccessLayer
{
   namespace observers
   {
      class CAcquisitionObservers final : public IAcquisitionObservers
      {
      public:
         ~CAcquisitionObservers() override = default;

         // IAcquisitionObservers Implementation
         void addObserver(boost::shared_ptr<INewAcquisitionObserver> observer) override;
         void removeObserver(boost::shared_ptr<INewAcquisitionObserver> observer) override;
         void onNewAcquisition(const boost::shared_ptr<database::entities::CAcquisition>&acquisition) override;
         // [END] IAcquisitionObservers Implementation

      private:
         std::vector<boost::shared_ptr<INewAcquisitionObserver>> m_observers;
      };
   } //namespace observers
} //namespace dataAccessLayer 
