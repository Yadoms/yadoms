#include "stdafx.h"
#include "ConditionRoot.h"
#include "ConditionFactory.h"

namespace job { namespace condition
{

enum { kConditionMet = shared::event::kUserFirstId };

CConditionRoot::CConditionRoot(const shared::CDataContainer& conditionConfiguration, const IConditionFactory& conditionFactory)
   :m_condition(conditionFactory.createCondition(conditionConfiguration))
{
}

CConditionRoot::~CConditionRoot()
{         
}

void CConditionRoot::wait()
{
   int event = m_eventHandler.waitForEvents();
   BOOST_ASSERT(event == kConditionMet);
}

void CConditionRoot::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver)
{
   m_condition->registerToNotificationCenter(notificationObserver, shared_from_this());
}

void CConditionRoot::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver)
{
   m_condition->unregisterFromNotificationCenter(notificationObserver);
}

void CConditionRoot::onKeywordStateChange()
{
   if(m_condition->eval())
      m_eventHandler.postEvent(kConditionMet);
}

} } // namespace job::condition	
	
	