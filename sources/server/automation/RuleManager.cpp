#include "stdafx.h"
#include "RuleManager.h"
#include "RuleStateHandler.h"
#include "database/IRuleRequester.h"
#include "Rule.h"
#include "script/Factory.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>
#include "RuleException.hpp"

namespace automation
{

CRuleManager::CRuleManager(boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
   boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger, boost::shared_ptr<shared::event::CEventHandler> supervisor, int ruleManagerEventId)
   :m_dbRequester(dbRequester),
   m_scriptFactory(new script::CFactory("scriptInterpreters", pluginGateway, configurationManager, dbAcquisitionRequester)),
   m_ruleStateHandler(new CRuleStateHandler(dbRequester, eventLogger, supervisor, ruleManagerEventId))
{
   startAllRules();
}

CRuleManager::~CRuleManager()
{
}

void CRuleManager::startAllRules()
{
   BOOST_ASSERT_MSG(m_startedRules.empty(), "Some rules are already started, are you sure that manager was successfuly stopped ?");

   // Start rules
   bool error = false;
   std::vector<boost::shared_ptr<database::entities::CRule> > rules = getRules();
   for (std::vector<boost::shared_ptr<database::entities::CRule> >::const_iterator it = rules.begin(); it != rules.end(); ++it)
   {
      try
      {
         // Don't start a rule in error state
         if ((*it)->State() != database::entities::ERuleState::kErrorValue)
            startRule((*it)->Id);
      }
      catch (CRuleException&)
      {
         YADOMS_LOG(error) << "Unable to start rule " << (*it)->Name() << ", skipped";
         error = true;
      }
   }

   if (error)
      m_ruleStateHandler->signalRulesStartError("One or more automation rules failed to start, check automation rules page for details");
}

std::vector<std::string> CRuleManager::getAvailableInterpreters()
{
   return m_scriptFactory->getAvailableInterpreters();
}

void CRuleManager::startRule(int ruleId)
{
   try
   {
      boost::shared_ptr<const database::entities::CRule> ruleData = getRule(ruleId);

      if (!ruleData->Enabled())
         return;  // Rule not enabled, don't start

      if (m_startedRules.find(ruleData->Id()) != m_startedRules.end())
         return;  // Rule already started

      m_ruleStateHandler->signalRuleStart(ruleId);

      boost::shared_ptr<IRule> newRule(new CRule(ruleData, m_scriptFactory, m_ruleStateHandler));
      m_startedRules[ruleId] = newRule;
      newRule->start();
   }
   catch(shared::exception::CEmptyResult& e)
   {
      const std::string& error((boost::format("Invalid rule %1%, element not found in database : %2%") % ruleId % e.what()).str());
      m_ruleStateHandler->signalRuleError(ruleId, error);
      throw CRuleException(error);
   }
   catch(shared::exception::CInvalidParameter& e)
   {
      const std::string& error((boost::format("Invalid rule %1% configuration, invalid parameter : %2%") % ruleId % e.what()).str());
      m_ruleStateHandler->signalRuleError(ruleId, error);
      throw CRuleException(error);
   }
   catch(shared::exception::COutOfRange& e)
   {
      const std::string& error((boost::format("Invalid rule %1% configuration, out of range : %2%") % ruleId % e.what()).str());
      m_ruleStateHandler->signalRuleError(ruleId, error);
      throw CRuleException(error);
   }
}

void CRuleManager::stopRule(int ruleId)
{
   std::map<int, boost::shared_ptr<IRule> >::iterator rule = m_startedRules.find(ruleId);

   if (rule == m_startedRules.end())
      return;

   m_startedRules.erase(rule);
}

std::vector<boost::shared_ptr<database::entities::CRule> > CRuleManager::getRules() const
{
   return m_dbRequester->getRules();
}

int CRuleManager::createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code)
{
   // Add rule in database
   int ruleId = m_dbRequester->addRule(ruleData);

   // Get the created rule from the id
   boost::shared_ptr<database::entities::CRule> updatedRuleData = m_dbRequester->getRule(ruleId);

   // Create script file
   m_scriptFactory->updateScriptFile(updatedRuleData, code);

   // Start the rule
   startRule(ruleId);

   return ruleId;   
}

boost::shared_ptr<database::entities::CRule> CRuleManager::getRule(int id) const
{
    return m_dbRequester->getRule(id);
}

std::string CRuleManager::getRuleCode(int id) const
{
   try
   {
      boost::shared_ptr<database::entities::CRule> ruleData(m_dbRequester->getRule(id));
      return m_scriptFactory->getScriptFile(ruleData);
   }
   catch(shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Unable to get rule code : " << e.what();
      return std::string();
   }
}

void CRuleManager::updateRule(boost::shared_ptr<const database::entities::CRule> ruleData)
{
   // Check for supported modifications
   if (!ruleData->Id.isDefined())
   {
      BOOST_ASSERT(false); // ID must be provided
      throw new shared::exception::CException("Update rule : rule ID was not provided");
   }

   m_dbRequester->updateRule(ruleData);

   if (ruleData->Enabled.isDefined() && m_dbRequester->getRule(ruleData->Id)->State() != database::entities::ERuleState::kErrorValue)
   {
      if (ruleData->Enabled() ^ (m_startedRules.find(ruleData->Id()) != m_startedRules.end()))
      {
         // Enable/disable state changed, apply it
         if (ruleData->Enabled())
            startRule(ruleData->Id());
         else
            stopRule(ruleData->Id());
      }
   }
}

void CRuleManager::updateRuleCode(int id, const std::string& code)
{
   // If rule was started, must be stopped to update its configuration
   if (m_startedRules.find(id) != m_startedRules.end())
      stopRule(id);

   // Update script file
   boost::shared_ptr<database::entities::CRule> ruleData(m_dbRequester->getRule(id));
   m_scriptFactory->updateScriptFile(ruleData, code);

   // Restart rule
   startRule(id);
}

void CRuleManager::deleteRule(int id)
{
   try
   {
      boost::shared_ptr<database::entities::CRule> ruleData(m_dbRequester->getRule(id));

      // Stop the rule
      stopRule(id);

      // Remove in database
      m_dbRequester->deleteRule(id);

      // Remove script file
      m_scriptFactory->deleteScriptFile(ruleData);
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Unable to delete rule (" << id << ") : " << e.what();
      throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(id));
   }
}

void CRuleManager::restartRule(int id)
{
   if (m_startedRules.find(id) != m_startedRules.end())
      throw CRuleException((boost::format("Rule %1% already started") % id).str());

   boost::shared_ptr<const database::entities::CRule> ruleData(m_dbRequester->getRule(id));
   if (ruleData->State() != database::entities::ERuleState::kErrorValue)
      throw CRuleException((boost::format("Rule %1% not in error state, can not be restarted") % id).str());

   startRule(id);
}

void CRuleManager::signalEvent(const CManagerEvent& event)
{
   switch (event.getSubEventId())
   {
   case CManagerEvent::kRuleAbnormalStopped:
   {
      // The rule has stopped in a non-conventional way (probably crashed)

      // First perform the full stop
      stopRule(event.getRuleId());

      break;
   }
   default:
   {
      YADOMS_LOG(error) << "Unknown message id";
      BOOST_ASSERT(false);
      break;
   }
   }
}


} // namespace automation	
	
	