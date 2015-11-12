#include "stdafx.h"
#include <shared/Log.h>
#include "RuleStateHandler.h"
#include "RuleException.hpp"
#include "ManagerEvent.h"

namespace automation
{

CRuleStateHandler::CRuleStateHandler(boost::shared_ptr<database::IRuleRequester> dbRequester,
   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
   boost::shared_ptr<shared::event::CEventHandler> supervisor, int ruleManagerEventId)
   :m_ruleRequester(dbRequester), m_eventLogger(eventLogger), m_supervisor(supervisor), m_ruleManagerEventId(ruleManagerEventId)
{
}

CRuleStateHandler::~CRuleStateHandler()
{
}

void CRuleStateHandler::signalNormalRuleStop(int ruleId)
{
   YADOMS_LOG(information) << "Stop rule #" << ruleId;

   // Signal the stop to asynchronously remove from running rules list
   postToSupervisor(CManagerEvent::kRuleStopped, ruleId);
}

void CRuleStateHandler::signalRuleError(int ruleId, const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailedValue, m_ruleRequester->getRule(ruleId)->Name(), error);

   // Signal the abnormal stop to asynchronously remove from list
   postToSupervisor(CManagerEvent::kRuleAbnormalStopped, ruleId, error);
}

void CRuleStateHandler::signalRulesStartError(int ruleId, const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailedValue, "Automation rules", error);

   // Signal the abnormal stop to asynchronously remove from list
   postToSupervisor(CManagerEvent::kRuleAbnormalStopped, ruleId, error);
}

void CRuleStateHandler::postToSupervisor(const CManagerEvent::ESubEventType& eventType, int ruleId, const std::string& error) const
{
   CManagerEvent event(eventType, ruleId, error);
   m_supervisor->postEvent<CManagerEvent>(m_ruleManagerEventId, event);
}


} // namespace automation	
	
	