#include "stdafx.h"
#include "Empty.h"

namespace job { namespace condition
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

void CEmpty::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> /*notificationObserver*/, boost::shared_ptr<IConditionRootUpdater> /*conditionRootNotifier*/)
{
}

void CEmpty::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> /*notificationObserver*/)
{
}

} } // namespace job::condition	
	
	