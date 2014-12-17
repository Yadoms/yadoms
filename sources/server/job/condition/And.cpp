#include "stdafx.h"
#include "And.h"
#include "ConditionFactory.h"

namespace job { namespace condition
{

CAnd::CAnd(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory, IConditionRoot& conditionRoot, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver)
{
   for (std::vector<shared::CDataContainer>::const_iterator it = operands.begin(); it != operands.end(); ++it)
      m_operands.push_back(conditionFactory.createCondition(*it, conditionRoot, notificationObserver));
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

} } // namespace job::condition	
	
	