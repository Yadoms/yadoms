#include "stdafx.h"
#include "LogEventFrame.h"

namespace web
{
   namespace ws
   {
      CLogEventFrame::CLogEventFrame(boost::shared_ptr<const database::entities::CEventLogger> logEvent)
         : CFrameBase(EFrameType::kLogEventNew)
      {
         m_internalContainer.set("date", logEvent->Date());
         m_internalContainer.set("code", logEvent->Code());
         m_internalContainer.set("who", logEvent->Who());
         m_internalContainer.set("what", logEvent->What());
      }

      CLogEventFrame::~CLogEventFrame()
      {
      }
   } //namespace ws
} //namespace web


