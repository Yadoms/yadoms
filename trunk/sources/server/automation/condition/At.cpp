#include "stdafx.h"
#include "At.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/event/Now.h>

namespace automation { namespace condition
{

CAt::CAt(const boost::posix_time::ptime& timePoint)
   :m_conditionIsMet(false),
   m_timePoint(shared::event::now().date(), timePoint.time_of_day())   
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

   m_conditionIsMet = false;
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
	
	
