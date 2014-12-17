#include "stdafx.h"
#include "ConditionRoot.h"
#include "ConditionFactory.h"

namespace job { namespace condition
{

enum { kConditionMet = shared::event::kUserFirstId };

CConditionRoot::CConditionRoot(const shared::CDataContainer& conditionConfiguration, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, const IConditionFactory& conditionFactory)
{
   conditionFactory.createCondition(conditionConfiguration, *this, notificationObserver);
}

CConditionRoot::~CConditionRoot()
{         
}

void CConditionRoot::eval()
{
   if(m_condition->eval())
      m_eventHandler.postEvent(kConditionMet);
}

void CConditionRoot::wait()
{
   int event = m_eventHandler.waitForEvents();
   BOOST_ASSERT(event == kConditionMet);
}

} } // namespace job::condition	
	
	