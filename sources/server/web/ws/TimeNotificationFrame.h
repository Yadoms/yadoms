#pragma once
#include "FrameBase.h"

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be send to GUI to notify for task update
      //-----------------------------
      class CTimeNotificationFrame : public CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param [in]    time The time to send
         //-----------------------------
         explicit CTimeNotificationFrame(const boost::posix_time::ptime& time);

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CTimeNotificationFrame();
      };
   } //namespace ws
} //namespace web