#pragma once

#include "FrameBase.h"
#include "database/entities/Entities.h"


namespace web { namespace ws {

   //-----------------------------
   ///\brief Frame which can be send to GUI to notify for new acquisitions
   //-----------------------------
   class CAcquisitionUpdateFrame : public CFrameBase
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param[in] acquisition     The acquisition to send to GUI
      ///\param[in] dailySummary    The new daily summary
      ///\param[in] hourlySummary   The new hourly summary
      //-----------------------------
      CAcquisitionUpdateFrame(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>(),
         boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>());

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CAcquisitionUpdateFrame();

   protected:
      //-----------------------------
      ///\bruef The acquisition field name
      //-----------------------------
      static const std::string m_acquisitionString;
      //-----------------------------
      ///\bruef The acquisition SUMMARY DAY field name
      //-----------------------------
      static const std::string m_acquisitionDayString;
      //-----------------------------
      ///\bruef The acquisition SUMMARY HOUR name
      //-----------------------------
      static const std::string m_acquisitionHourString;
   };

} //namespace ws
} //namespace web