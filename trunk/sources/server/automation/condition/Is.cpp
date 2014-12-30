#include "stdafx.h"
#include "Is.h"
#include <shared/Log.h>

namespace automation { namespace condition
{
   DECLARE_ENUM_IMPLEMENTATION(EOperators,
      ((Equal))
      ((Different))
      ((Lower))
      ((LowerOrEqual))
      ((Greater))
      ((GreaterOrEqual))
   )

CIs::CIs(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_keywordId(configuration.get<int>("keywordId")),
   m_operator(configuration.get<EOperators>("operator")),
   m_expectedValue(configuration.get<std::string>("refValue")),
   m_value(dbAcquisitionRequester->getKeywordLastData(m_keywordId)->Value)
{
}

CIs::~CIs()
{
}

bool CIs::eval() const
{
   switch (m_operator)
   {
   case EOperators::kEqualValue: return m_value == m_expectedValue;
   case EOperators::kDifferentValue: return m_value != m_expectedValue;
   case EOperators::kLowerValue: return toDouble(m_value) < toDouble(m_expectedValue);
   case EOperators::kLowerOrEqualValue: return toDouble(m_value) <= toDouble(m_expectedValue);
   case EOperators::kGreaterValue: return toDouble(m_value) > toDouble(m_expectedValue);
   case EOperators::kGreaterOrEqualValue: return toDouble(m_value) >= toDouble(m_expectedValue);
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
	
	