#include "stdafx.h"
#include "Is.h"

namespace job { namespace condition
{

CIs::CIs(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_keywordId(configuration.get<int>("keywordId")),
   m_state(dbAcquisitionRequester->getKeywordLastData(m_keywordId)->Value),
   m_expectedState(configuration.get<std::string>("expectedState"))
{
}

CIs::~CIs()
{
}

bool CIs::eval() const
{
   return m_state == m_expectedState;
}

void CIs::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   notificationObserver->registerKeywordUpdater(shared_from_this(), conditionRootNotifier);
}

void CIs::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver)
{
   notificationObserver->unregisterKeywordUpdater(shared_from_this());
}

int CIs::getKeywordId() const
{
   return m_keywordId;
}

void CIs::onKeywordStateChange(const std::string& state)
{
   m_state = state;
}

} } // namespace job::condition	
	
	