#pragma once
#include "FrameBase.h"
#include "database/entities/Entities.h"

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be send to GUI to notify for new acquisitions
      //-----------------------------
      class CAcquisitionUpdateFrame : public CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param[in] acquisition     The acquisition to send to GUI
         //-----------------------------
         explicit CAcquisitionUpdateFrame(boost::shared_ptr<const database::entities::CAcquisition> acquisition);

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CAcquisitionUpdateFrame();
      };
   } //namespace ws
} //namespace web


