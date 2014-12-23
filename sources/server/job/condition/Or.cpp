#include "stdafx.h"
#include "Or.h"
#include "ConditionFactory.h"

namespace job { namespace condition
{

COr::COr(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory)
{
   for (std::vector<shared::CDataContainer>::const_iterator it = operands.begin(); it != operands.end(); ++it)
      m_operands.push_back(conditionFactory.createCondition(*it));
}

COr::~COr()
{         
}

bool COr::eval() const
{
   for (Operands::const_iterator it = m_operands.begin(); it != m_operands.end(); ++it)
      if ( (*it)->eval() )
         return true;
   return false;
}

void COr::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   for (Operands::const_iterator it = m_operands.begin(); it != m_operands.end(); ++it)
      (*it)->registerToNotificationCenter(notificationObserver, conditionRootNotifier);
}

void COr::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver)
{
   for (Operands::const_iterator it = m_operands.begin(); it != m_operands.end(); ++it)
      (*it)->unregisterFromNotificationCenter(notificationObserver);
}

} } // namespace job::condition	
	
	