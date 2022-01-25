#include "stdafx.h"

#include "AcquisitionObservers.h"

namespace dataAccessLayer
{
   namespace observers
   {
      void CAcquisitionObservers::addObserver(boost::shared_ptr<INewAcquisitionObserver> observer)
      {
         m_observers.push_back(observer);
      }

      void CAcquisitionObservers::removeObserver(boost::shared_ptr<INewAcquisitionObserver> observer)
      {
         const auto& toErase = std::find(m_observers.begin(),
                                         m_observers.end(),
                                         observer);
         if (toErase == m_observers.end())
            return;
         m_observers.erase(toErase);
      }

      void CAcquisitionObservers::onNewAcquisition(const boost::shared_ptr<database::entities::CAcquisition>& acquisition)
      {
         for (const auto& observer : m_observers)
            observer->onNewAcquisition(acquisition);
      }
   } //namespace observers
} //namespace dataAccessLayer 
