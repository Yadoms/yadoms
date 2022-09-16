#include "stdafx.h"
#include "TimeNotificationFrame.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         CTimeNotificationFrame::CTimeNotificationFrame(const boost::posix_time::ptime& time)
            : CFrameBase(EFrameType::kTimeNotification)
         {
            m_internalContainer.set("time", time);
         }

         CTimeNotificationFrame::~CTimeNotificationFrame()
         {
         }
      } //namespace ws
   } //namespace poco
} //namespace web
