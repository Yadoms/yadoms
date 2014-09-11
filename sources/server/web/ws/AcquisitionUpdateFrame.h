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
      ///\param [in]    content     The acquisition to send to GUI
      //-----------------------------
      CAcquisitionUpdateFrame(const database::entities::CAcquisition & content);

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CAcquisitionUpdateFrame();
   };

} //namespace ws
} //namespace web