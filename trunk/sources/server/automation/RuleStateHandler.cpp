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

void CRuleStateHandler::signalRuleStart(int ruleId)
{
   // Record event
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->State = database::entities::ERuleState::kRunningValue;
   ruleData->StartDate = shared::currentTime::Provider::now();
   ruleData->ErrorMessage = std::string();
   m_ruleRequester->updateRule(ruleData);

   YADOMS_LOG(information) << "Start rule #" << ruleId;
}

void CRuleStateHandler::signalNormalRuleStop(int ruleId)
{
   // Record event
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->State = database::entities::ERuleState::kStoppedValue;
   ruleData->StopDate = shared::currentTime::Provider::now();
   ruleData->ErrorMessage = std::string();
   m_ruleRequester->updateRule(ruleData);

   YADOMS_LOG(information) << "Stop rule #" << ruleId;
}

void CRuleStateHandler::signalNormalRuleStopAndDisable(int ruleId)
{
   // Record event
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->Enabled = false;
   ruleData->State = database::entities::ERuleState::kStoppedValue;
   ruleData->StopDate = shared::currentTime::Provider::now();
   ruleData->ErrorMessage = std::string();
   m_ruleRequester->updateRule(ruleData);

   YADOMS_LOG(information) << "Stop and disable rule #" << ruleId;
}

void CRuleStateHandler::signalRuleError(int ruleId, const std::string& error)
{
   // Record error
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->State = database::entities::ERuleState::kErrorValue;
   ruleData->ErrorMessage = error;
   ruleData->StopDate = shared::currentTime::Provider::now();
   m_ruleRequester->updateRule(ruleData);

   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailedValue, m_ruleRequester->getRule(ruleId)->Name(), error);

   // Signal the abnormal stop to asynchronously remove from list
   CManagerEvent event(CManagerEvent::kRuleAbnormalStopped, ruleId);
   m_supervisor->postEvent<CManagerEvent>(m_ruleManagerEventId, event);
}

void CRuleStateHandler::signalRulesStartError(const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailedValue, "Automation rules", error);
}

} // namespace automation	
	
	