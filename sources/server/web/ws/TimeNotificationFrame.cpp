#include "stdafx.h"
#include "TimeNotificationFrame.h"

namespace web
{
   namespace ws
   {
      CTimeNotificationFrame::CTimeNotificationFrame(const boost::posix_time::ptime& time)
         :CFrameBase(EFrameType::kTimeNotification)
      {
         m_internalContainer.set("time", time);
      }

      CTimeNotificationFrame::~CTimeNotificationFrame()
      {
      }
   } //namespace ws
} //namespace web