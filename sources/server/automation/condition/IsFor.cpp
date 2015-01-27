#include "stdafx.h"
#include "IsFor.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>

namespace automation { namespace condition
{

CIsFor::CIsFor(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_keywordId(configuration.get<int>("keywordId")),
   m_operator(configuration.get<EComparators>("operator")),
   m_value(dbAcquisitionRequester->getKeywordLastData(m_keywordId)->Value),
   m_expectedValue(configuration.get<std::string>("refValue")),
   m_previousState(false),
   m_conditionIsMet(false),
   m_duration(configuration.get<boost::posix_time::time_duration>("for"))
{
}

CIsFor::~CIsFor()
{
}

bool CIsFor::eval()
{
   return m_conditionIsMet;
}

bool CIsFor::evalState() const
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

void CIsFor::registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier)
{
   notificationObserver->registerKeywordUpdater(shared_from_this(), conditionRootNotifier);
   m_ConditionRootNotifier = conditionRootNotifier;
}

void CIsFor::unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver)
{
   m_ConditionRootNotifier.reset();
   notificationObserver->unregisterKeywordUpdater(shared_from_this());
}

int CIsFor::getKeywordId() const
{
   return m_keywordId;
}

void CIsFor::onKeywordStateChange(const std::string& state)
{
   m_value = state;

   bool newState = evalState();
   if (newState == m_previousState)
      return;

   m_previousState = newState;

   if (!newState)
   {
      m_conditionIsMet = false;
      stopTimer();
      return;
   }

   startTimer();
}

void CIsFor::onTimer()
{
   m_conditionIsMet = true;
   if (m_ConditionRootNotifier)
      m_ConditionRootNotifier->requestEvaluation();
}

void CIsFor::startTimer()
{
   m_timerThread.start(m_duration, boost::bind(&CIsFor::onTimer, this));
}

void CIsFor::stopTimer()
{
   m_timerThread.stop();
}

void CIsFor::IsForThread::doWork()
{
   try
   {
      enum { kTimerEventId = shared::event::kUserFirstId };
      shared::event::CEventHandler eventHandler;
      eventHandler.createTimer(kTimerEventId, shared::event::CEventTimer::kOneShot, m_duration);

      while (true)
      {
         // Wait for an event
         switch (eventHandler.waitForEvents())
         {
         case kTimerEventId:
            m_timeoutCallback();
            break;
         default:
            YADOMS_LOG(error) << "Invalid event ID received " << eventHandler.getEventId();
            break;
         }
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

double CIsFor::toDouble(const std::string& value)
{
   return boost::lexical_cast<double>(value);
}

} } // namespace automation::condition	
	
	
