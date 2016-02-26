#pragma once

#include "../INotification.h"
#include "../change/Notification.hpp"

namespace notification { namespace summary {
   
   //-----------------------------
   ///\brief Interface for basic notification (handle a typed object)
   //-----------------------------
   class CNotification : public INotification
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in] obj   The notification object
      //-----------------------------
      explicit CNotification(std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > & acquisitionSummary)
         :m_acquisitionSummary(acquisitionSummary)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNotification()
      {
      }

      //-----------------------------
      ///\brief All the summary data computed
      //-----------------------------
      const std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > & getAcquisitionSummaries() const
      {
         return m_acquisitionSummary;
      }
      
   private:
      //-----------------------------
      ///\brief All the summary data computed
      //-----------------------------
      std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > m_acquisitionSummary;      
   };
   
} //namespace summary
} //namespace notification