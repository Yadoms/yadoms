#include "stdafx.h"
#include "TimeNotificationFrame.h"
#include <shared/Log.h>

namespace web
{
   namespace ws
   {
      const std::string CTimeNotificationFrame::m_time = "time";   

      CTimeNotificationFrame::CTimeNotificationFrame(const boost::posix_time::ptime& time)
         :CFrameBase(EFrameType::kTaskUpdateNotification)
      {
         m_internalContainer.set(m_time, time);
      }

      CTimeNotificationFrame::~CTimeNotificationFrame()
      {
      }
   } //namespace ws
} //namespace web