#include "stdafx.h"
#include <shared/shared/event/Now.h>
#include <shared/Log.h>
#include "RuleErrorHandler.h"
#include "RuleException.hpp"

namespace automation
{

CRuleErrorHandler::CRuleErrorHandler(boost::shared_ptr<database::IRuleRequester> ruleRequester, boost::shared_ptr<database::IEventLoggerRequester> eventLoggerRequester)
   :m_ruleRequester(ruleRequester), m_eventLoggerRequester(eventLoggerRequester)
{
}

CRuleErrorHandler::~CRuleErrorHandler()
{
}

void CRuleErrorHandler::signalRuleStart(int ruleId)
{
   // Record event
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->State = database::entities::ERuleState::kRunningValue;
   ruleData->StartDate = shared::event::now();
   ruleData->ErrorMessage = std::string();
   m_ruleRequester->updateRule(ruleData);

   YADOMS_LOG(information) << "Start rule #" << ruleId;
}

void CRuleErrorHandler::signalNormalRuleStop(int ruleId)
{
   // Record event
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->State = database::entities::ERuleState::kStoppedValue;
   ruleData->StopDate = shared::event::now();
   ruleData->ErrorMessage = std::string();
   m_ruleRequester->updateRule(ruleData);

   YADOMS_LOG(information) << "Stop rule #" << ruleId;
}

void CRuleErrorHandler::signalRuleError(int ruleId, const std::string& error)
{
   // Record error
   boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   ruleData->Id = ruleId;
   ruleData->State = database::entities::ERuleState::kErrorValue;
   ruleData->ErrorMessage = error;
   ruleData->StopDate = shared::event::now();
   m_ruleRequester->updateRule(ruleData);

   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLoggerRequester->addEvent(database::entities::ESystemEventCode::kRuleFailedValue, ruleData->Name(), error);
}

void CRuleErrorHandler::signalRulesStartError(const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLoggerRequester->addEvent(database::entities::ESystemEventCode::kRuleFailedValue, "Automation rules", error);
}

} // namespace automation	
	
	