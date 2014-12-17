#include "stdafx.h"
#include "Is.h"

namespace job { namespace condition
{

CIs::CIs(const shared::CDataContainer& configuration, IConditionRoot& conditionRoot, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver)
   :m_keywordId(configuration.get<int>("keywordId")), m_expectedState(configuration.get<std::string>("expectedState")),
   m_notificationObserver(notificationObserver)
{
   //TODO initialiser m_state
}

CIs::~CIs()
{
   m_notificationObserver->unregisterKeywordUpdater(shared_from_this());
}

bool CIs::eval() const
{
   return m_state == m_expectedState;
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
	
	