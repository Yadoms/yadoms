#include "stdafx.h"
#include "IsForThread.h"
#include <shared/event/EventHandler.hpp>

namespace automation { namespace condition
{      

CIsForThread::CIsForThread()
   :shared::CThreadBase("IsForTimerThread")
{
}

CIsForThread::~CIsForThread()
{
}

void CIsForThread::start(const boost::posix_time::time_duration& duration, boost::function<void()> timeoutCallback)
{
   m_duration = duration;
   m_timeoutCallback = timeoutCallback;
   shared::CThreadBase::start();
}



void CIsForThread::doWork()
{
   try
   {
      enum { kTimerEventId = shared::event::kUserFirstId };
      shared::event::CEventHandler eventHandler;
      eventHandler.createTimer(kTimerEventId, shared::event::CEventTimer::kOneShot, m_duration);

      while (true)
      {
         // Wait for an event
         switch (eventHandler.waitForEvents())
         {
         case kTimerEventId:
            m_timeoutCallback();
            break;
         default:
            BOOST_ASSERT_MSG(false, "Invalid event ID received");
            break;
         }
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

} } // namespace automation::condition	
	
	