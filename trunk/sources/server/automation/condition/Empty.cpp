#include "stdafx.h"
#include "Empty.h"

namespace automation { namespace condition
{

CEmpty::CEmpty()
{
}

CEmpty::~CEmpty()
{         
}

bool CEmpty::eval() const
{
   return true;
}

void CEmpty::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> /*notificationObserver*/, boost::shared_ptr<IConditionRootUpdater> /*conditionRootNotifier*/)
{
}

void CEmpty::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> /*notificationObserver*/)
{
}

} } // namespace automation::condition	
	
	