#pragma once
#include "FrameBase.h"
#include "database/entities/Entities.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         //-----------------------------
         ///\brief Frame which can be send to GUI to notify for a new device created
         //-----------------------------
         class CNewDeviceFrame : public CFrameBase
         {
         public:
            //-----------------------------
            ///\brief Constructor
            ///\param [in]    content     The information to send to GUI (shared_ptr)
            //-----------------------------
            explicit CNewDeviceFrame(boost::shared_ptr<const database::entities::CDevice> content);

            //-----------------------------
            ///\brief Destructor
            //-----------------------------
            virtual ~CNewDeviceFrame();

         protected:
            //-----------------------------
            ///\bruef The acquisition field name
            //-----------------------------
            static const std::string m_device;
         };
      } //namespace ws
   } //namespace poco
} //namespace web
