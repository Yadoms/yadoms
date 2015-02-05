#pragma once

#include "FrameBase.h"
#include "database/entities/Entities.h"
#include "notifications/NewAcquisitionNotification.h"


namespace web { namespace ws {

   //-----------------------------
   ///\brief Frame which can be send to GUI to notify for new acquisitions
   //-----------------------------
   class CAcquisitionUpdateFrame : public CFrameBase
   {
   public:
      //-----------------------------
      ///\brief Constructor
      ///\param [in]    content     The acquisition to send to GUI
      //-----------------------------
      CAcquisitionUpdateFrame(const database::entities::CAcquisition & content);

      //-----------------------------
      ///\brief Constructor
      ///\param [in]    content     The acquisition to send to GUI (shared_ptr)
      //-----------------------------
      CAcquisitionUpdateFrame(boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData);

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CAcquisitionUpdateFrame();

   protected:
      //-----------------------------
      ///\bruef The acquisition field name
      //-----------------------------
      static const std::string m_acquition;
      //-----------------------------
      ///\bruef The acquisition SUMMARY DAY field name
      //-----------------------------
      static const std::string m_acquitionDay;
      //-----------------------------
      ///\bruef The acquisition SUMMARY HOUR name
      //-----------------------------
      static const std::string m_acquitionHour;
   };

} //namespace ws
} //namespace web