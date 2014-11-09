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
      ///\param [in]  acquisition   The acquisition data
      ///\param [in]  day           The dayly summary data
      ///\param [in]  jour          The hourly dummary data
      //-----------------------------
      CNewAcquisitionNotification(boost::shared_ptr< database::entities::CAcquisition > acquisition, boost::shared_ptr< database::entities::CAcquisitionSummary > day = boost::shared_ptr< database::entities::CAcquisitionSummary >(), boost::shared_ptr< database::entities::CAcquisitionSummary > hour = boost::shared_ptr< database::entities::CAcquisitionSummary > ())
         :m_acquisition(acquisition), m_daySummary(day), m_hourSummary(hour)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CNewAcquisitionNotification()
      {
      }

      //-----------------------------
      ///\brief Get the raw acquisition
      ///\return  the raw acquisition
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisition > getAcquisition() const { return m_acquisition; }

      //-----------------------------
      ///\brief Get the day summary data
      ///\return  the day summary data
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisitionSummary > getSummaryDay() const { return m_daySummary; }

      //-----------------------------
      ///\brief Get the hour summary data
      ///\return  the hour summary data
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisitionSummary > getSummaryHour() const { return m_hourSummary; }

   private:
      //-----------------------------
      ///\brief The raw acqusition
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisition > m_acquisition;

      //-----------------------------
      ///\brief The day summary data
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisitionSummary > m_daySummary;

      //-----------------------------
      ///\brief The hour summary data
      //-----------------------------
      boost::shared_ptr< database::entities::CAcquisitionSummary > m_hourSummary;
   };

} //namespace notifications 
