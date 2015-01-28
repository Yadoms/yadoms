#include "stdafx.h"
#include "At.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>

namespace automation { namespace condition
{

CAt::CAt(const boost::posix_time::ptime& timePoint)
   :m_timePoint(timePoint),
   m_conditionIsMet(false)
{
   startTimer();
}

CAt::~CAt()
{
}

bool CAt::eval()
{
   if (!m_conditionIsMet)
      return false;

   // Reset timer for next day
   m_conditionIsMet = false;
   startTimer();
   return true;
}

void CAt::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager>, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   m_ConditionRootNotifier = conditionRootNotifier;
}

void CAt::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager>)
{
   m_ConditionRootNotifier.reset();
}

void CAt::onTimer()
{
   m_conditionIsMet = true;
   if (m_ConditionRootNotifier)
      m_ConditionRootNotifier->requestEvaluation();
}

void CAt::startTimer()
{
   m_timerThread.start(m_timePoint, boost::bind(&CAt::onTimer, this));
}

void CAt::stopTimer()
{
   m_timerThread.stop();
}

} } // namespace automation::condition	
	
	
