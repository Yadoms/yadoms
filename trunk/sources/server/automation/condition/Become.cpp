#include "stdafx.h"
#include "Become.h"
#include <shared/Log.h>

namespace automation { namespace condition
{

CBecome::CBecome(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_keywordId(configuration.get<int>("keywordId")),
   m_operator(configuration.get<EComparators>("operator")),
   m_value(dbAcquisitionRequester->getKeywordLastData(m_keywordId)->Value),
   m_expectedValue(configuration.get<std::string>("refValue")),
   m_previousState(false)
{
}

CBecome::~CBecome()
{
}

bool CBecome::eval()
{
   bool newState = evalState();
   if (newState == m_previousState)
      return false;

   m_previousState = newState;

   if (!newState)
      return false;

   return true;
}

bool CBecome::evalState() const
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

void CBecome::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   notificationObserver->registerKeywordUpdater(shared_from_this(), conditionRootNotifier);
}

void CBecome::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   notificationObserver->unregisterKeywordUpdater(shared_from_this());
}

int CBecome::getKeywordId() const
{
   return m_keywordId;
}

void CBecome::onKeywordStateChange(const std::string& state)
{
   m_value = state;
}

double CBecome::toDouble(const std::string& value)
{
   return boost::lexical_cast<double>(value);
}

} } // namespace automation::condition	
	
	
