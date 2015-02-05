#include "stdafx.h"
#include "ConditionRoot.h"
#include "ConditionFactory.h"

namespace automation { namespace condition
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
   if (!!m_condition)
   {
      int event = m_eventHandler.waitForEvents();
      BOOST_ASSERT(event == kConditionMet);
   }
   else
   {
      // Need an interrupt point even if no condition
      boost::this_thread::interruption_point();
   }
}

void CConditionRoot::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   if (!!m_condition)
      m_condition->registerToNotificationCenter(notificationObserver, shared_from_this());
}

void CConditionRoot::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   if (!!m_condition)
      m_condition->unregisterFromNotificationCenter(notificationObserver);
}

void CConditionRoot::requestEvaluation()
{
   if(!!m_condition && m_condition->eval())
      m_eventHandler.postEvent(kConditionMet);
}

} } // namespace automation::condition	
	
	