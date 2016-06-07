#include "stdafx.h"
#include <shared/Log.h>
#include "RuleStateHandler.h"

namespace automation
{
   CRuleStateHandler::CRuleStateHandler(boost::shared_ptr<database::IRuleRequester> dbRequester,
                                        boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                        boost::shared_ptr<shared::event::CEventHandler> ruleEventHandler)
      : m_ruleRequester(dbRequester),
        m_eventLogger(eventLogger),
        m_ruleEventHandler(ruleEventHandler)
   {
   }

   CRuleStateHandler::~CRuleStateHandler()
   {
   }

   void CRuleStateHandler::signalNormalRuleStop(int ruleId)
   {
      YADOMS_LOG(information) << "Stop rule #" << ruleId;

      // Signal the stop to asynchronously remove from running rules list
      m_ruleEventHandler->postEvent(kRuleStopped, ruleId);
   }

   void CRuleStateHandler::signalRuleError(int ruleId, const std::string& error)
   {
      // Signal error
      YADOMS_LOG(error) << error;
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed, m_ruleRequester->getRule(ruleId)->Name(), error);

      // Signal the abnormal stop to asynchronously remove from list
      m_ruleEventHandler->postEvent(kRuleAbnormalStopped, std::pair<int, std::string>(ruleId, error));
   }

   void CRuleStateHandler::signalRulesStartError(int ruleId, const std::string& error)
   {
      // Signal error
      YADOMS_LOG(error) << error;
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed, "Automation rules", error);

      // Signal the abnormal stop to asynchronously remove from list
      m_ruleEventHandler->postEvent(kRuleAbnormalStopped, std::pair<int, std::string>(ruleId, error));
   }
} // namespace automation	


