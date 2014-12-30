#include "stdafx.h"
#include "And.h"
#include "ConditionFactory.h"

namespace automation { namespace condition
{

CAnd::CAnd(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory)
{
   for (std::vector<shared::CDataContainer>::const_iterator it = operands.begin(); it != operands.end(); ++it)
      m_operands.push_back(conditionFactory.createCondition(*it));
}

CAnd::~CAnd()
{         
}

bool CAnd::eval() const
{
   for (Operands::const_iterator it = m_operands.begin(); it != m_operands.end(); ++it)
      if ( ! (*it)->eval() )
         return false;
   return true;
}

void CAnd::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   for (Operands::const_iterator it = m_operands.begin(); it != m_operands.end(); ++it)
      (*it)->registerToNotificationCenter(notificationObserver, conditionRootNotifier);
}

void CAnd::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   for (Operands::const_iterator it = m_operands.begin(); it != m_operands.end(); ++it)
      (*it)->unregisterFromNotificationCenter(notificationObserver);
}

} } // namespace automation::condition	
	
	