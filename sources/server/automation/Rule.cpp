#include "stdafx.h"
#include "Rule.h"
#include "condition/ConditionRoot.h"
#include "ActionList.h"

namespace automation
{

CRule::CRule(const database::entities::CRule& ruleData,
   boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver,
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   condition::IConditionFactory& conditionFactory,
   boost::shared_ptr<action::script::IFactory> scriptFactory)
   :m_name(ruleData.Name()),
   m_id(ruleData.Id()),
   //TODO : update those two lines (first parameters are set to an empty CDataContainer)
   m_condition(new condition::CConditionRoot(shared::CDataContainer(), conditionFactory)),
   m_actions(new CActionList(shared::CDataContainer(), pluginGateway, scriptFactory)),
   m_notificationObserver(notificationObserver)
{
   m_condition->registerToNotificationCenter(m_notificationObserver);
}

CRule::~CRule()
{
   m_condition->unregisterFromNotificationCenter(m_notificationObserver);
   stop();
}

void CRule::start()
{
   m_thread.reset(new CRuleThread(m_name, *this));
   m_thread->start();
}

void CRule::stop()
{
   m_actions->stopPending();
   m_thread->stop();
   m_thread.reset();
}

void CRule::doWork()
{
   try
   {
      while (true)
      {
         m_condition->wait();
         m_actions->doAll();
         boost::this_thread::sleep(boost::posix_time::millisec(100)); // To avoid CPU load
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

} // namespace automation	
	
	