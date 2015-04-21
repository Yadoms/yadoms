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
      CNotification(boost::shared_ptr<database::entities::CAcquisition> acquisition, boost::shared_ptr<database::entities::CAcquisitionSummary> daily, boost::shared_ptr<database::entities::CAcquisitionSummary> hourly)
         :change::CNotification<database::entities::CAcquisition>(acquisition, notification::change::EChangeType::kCreate), m_daily(daily), m_hourly(hourly)
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

      //-----------------------------
      ///\brief Get the daily acquisition
      ///\return The daily acquisition
      //-----------------------------
      boost::shared_ptr<database::entities::CAcquisitionSummary> getDailyAcquisition()
      {
         return m_daily;
      }

      //-----------------------------
      ///\brief Get the hourly acquisition
      ///\return The hourly acquisition
      //-----------------------------
      boost::shared_ptr<database::entities::CAcquisitionSummary> getHourlyAcquisition()
      {
         return m_hourly;
      }

   private:
      //-----------------------------
      ///\brief The notification objects
      //-----------------------------
      boost::shared_ptr<database::entities::CAcquisitionSummary> m_daily;
      boost::shared_ptr<database::entities::CAcquisitionSummary> m_hourly;
   };

} //namespace acquisition
} //namespace notification