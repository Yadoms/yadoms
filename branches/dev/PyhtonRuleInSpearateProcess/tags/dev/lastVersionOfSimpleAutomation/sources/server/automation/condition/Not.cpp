#include "stdafx.h"
#include "Not.h"
#include "ConditionFactory.h"

namespace automation { namespace condition
{

CNot::CNot(const shared::CDataContainer& operand, const IConditionFactory& conditionFactory)
   :m_operand(conditionFactory.createCondition(operand))
{
}

CNot::~CNot()
{         
}

bool CNot::eval()
{
   return !m_operand->eval();
}

void CNot::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   m_operand->registerToNotificationCenter(notificationObserver, conditionRootNotifier);
}

void CNot::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   m_operand->unregisterFromNotificationCenter(notificationObserver);
}

} } // namespace automation::condition	
	
	