#include "stdafx.h"
#include "Is.h"
#include <shared/Log.h>

namespace automation { namespace condition
{

CIs::CIs(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_keywordId(configuration.get<int>("keywordId")),
   m_operator(configuration.get<EComparators>("operator")),
   m_value(dbAcquisitionRequester->getKeywordLastData(m_keywordId)->Value),
   m_expectedValue(configuration.get<std::string>("refValue"))
{
}

CIs::~CIs()
{
}

bool CIs::eval()
{
   switch (m_operator)
   {
   case EComparators::kEqualValue: return m_value == m_expectedValue;
   case EComparators::kDifferentValue: return m_value != m_expectedValue;
   case EComparators::kLowerValue: return toDouble(m_value) < toDouble(m_expectedValue);
   case EComparators::kLowerOrEqualValue: return toDouble(m_value) <= toDouble(m_expectedValue);
   case EComparators::kGreaterValue: return toDouble(m_value) > toDouble(m_expectedValue);
   case EComparators::kGreaterOrEqualValue: return toDouble(m_value) >= toDouble(m_expectedValue);
   default:
      YADOMS_LOG(error) << "Invalid operator " << m_operator;
      throw shared::exception::CInvalidParameter(m_operator);
   }   
}

void CIs::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   notificationObserver->registerKeywordUpdater(shared_from_this(), conditionRootNotifier);
}

void CIs::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   notificationObserver->unregisterKeywordUpdater(shared_from_this());
}

int CIs::getKeywordId() const
{
   return m_keywordId;
}

void CIs::onKeywordStateChange(const std::string& state)
{
   m_value = state;
}

double CIs::toDouble(const std::string& value)
{
   return boost::lexical_cast<double>(value);
}

} } // namespace automation::condition	
	
	
