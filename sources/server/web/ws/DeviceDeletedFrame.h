#pragma once
#include "FrameBase.h"
#include "database/entities/Entities.h"

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be send to GUI to notify for a new device created
      //-----------------------------
      class CDeviceDeletedFrame : public CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param [in]    content     The information to send to GUI (shared_ptr)
         //-----------------------------
         explicit CDeviceDeletedFrame(boost::shared_ptr<const database::entities::CDevice> content);

         virtual ~CDeviceDeletedFrame() = default;

      protected:
         //-----------------------------
         ///\bruef The acquisition field name
         //-----------------------------
         static const std::string Device;
      };
   } //namespace ws
} //namespace web
