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
   boost::shared_ptr<action::IScriptInterpreterFactory> scriptInterpreterFactory)
   :m_name(ruleData.Name()),
   m_id(ruleData.Id()),
   m_condition(new condition::CConditionRoot(ruleData.Triggers(), conditionFactory)),
   m_actions(new CActionList(ruleData.Actions(), pluginGateway, scriptInterpreterFactory)),
   m_notificationObserver(notificationObserver)
{
   m_condition->registerToNotificationCenter(m_notificationObserver);
}

CRule::~CRule()
{
   stop();
   m_condition->unregisterFromNotificationCenter(m_notificationObserver);
}

void CRule::start()
{
   m_thread.reset(new CRuleThread(m_name, *this));
   m_thread->start();
}

void CRule::stop()
{
   m_thread->stop();
   m_thread.reset();
}

void CRule::doWork()
{
   while (true)
   {
      m_condition->wait();
      m_actions->doAll();
   }
}

} // namespace automation	
	
	