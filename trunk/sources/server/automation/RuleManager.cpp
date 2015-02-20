#include "stdafx.h"
#include "RuleManager.h"
#include "database/IRuleRequester.h"
#include "Rule.h"
#include "script/Factory.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>
#include "RuleException.hpp"

namespace automation
{

CRuleManager::CRuleManager(boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_dbRequester(dbRequester),
   m_scriptFactory(new script::CFactory("scriptInterpreters", pluginGateway, notificationCenter, dbAcquisitionRequester))
{        
}

CRuleManager::~CRuleManager()
{        
}

void CRuleManager::start()
{
   BOOST_ASSERT_MSG(m_startedRules.empty(), "Some rules are already started, are you sure that manager was successfuly stopped ?");

   // Start rules
   std::vector<boost::shared_ptr<database::entities::CRule> > rules = getRules();
   for (std::vector<boost::shared_ptr<database::entities::CRule> >::const_iterator it = rules.begin(); it != rules.end(); ++it)
   {
      try
      {
         startRule((*it)->Id);     
      }
      catch (CRuleException&)
      {
         YADOMS_LOG(error) << "Unable to start rule " << (*it)->Name() << ", skipped";
      }
   }
}

void CRuleManager::stop()
{
   // Free all rules (will stop rules)
   m_startedRules.clear();
}

void CRuleManager::startRule(int ruleId)
{
   try
   {
      boost::shared_ptr<const database::entities::CRule> ruleData = getRule(ruleId);

      if (!ruleData->Enabled())
         return;  // Rule not enabled, don't start

      boost::shared_ptr<IRule> newRule(new CRule(ruleData, m_scriptFactory));
      m_startedRules[ruleId] = newRule;
      newRule->start();
   }
   catch(shared::exception::CEmptyResult& e)
   {
      std::string msg((boost::format("Invalid rule %1%, element not found in database : %2%") % ruleId % e.what()).str());
      YADOMS_LOG(error) << msg;
      throw CRuleException(msg);
   }
   catch(shared::exception::CInvalidParameter& e)
   {
      std::string msg((boost::format("Invalid rule %1% configuration, invalid parameter : %2%") % ruleId % e.what()).str());
      YADOMS_LOG(error) << msg;
      throw CRuleException(msg);
   }
   catch(shared::exception::COutOfRange& e)
   {
      std::string msg((boost::format("Invalid rule %1% configuration, out of range : %2%") % ruleId % e.what()).str());
      YADOMS_LOG(error) << msg;
      throw CRuleException(msg);
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

int CRuleManager::createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string & scriptCode)
{
   // Add rule in database
   int ruleId = m_dbRequester->addRule(ruleData);

   // Update rule data with the rule ID
   boost::shared_ptr<database::entities::CRule> updatedRuleData(new database::entities::CRule(*ruleData));
   updatedRuleData->Id = ruleId;

   // Create script file
   m_scriptFactory->updateScriptFile(updatedRuleData, scriptCode);

   // Start the rule
   startRule(ruleId);

   return ruleId;   
}

boost::shared_ptr<database::entities::CRule> CRuleManager::getRule(int id) const
{
    return m_dbRequester->getRule(id);
}

const std::string CRuleManager::getRuleCode(int id) const
{
   return m_scriptFactory->getScriptFile(getRule(id));
}


void CRuleManager::updateRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string & scriptCode)
{
   // Check for supported modifications
   if (!ruleData->Id.isDefined())
   {
      BOOST_ASSERT(false); // ID must be provided
      throw new shared::exception::CException("Update rule : rule ID was not provided");
   }

   // If rule was started, must be stopped to update its configuration
   if (m_startedRules.find(ruleData->Id()) != m_startedRules.end())
      stopRule(ruleData->Id());

   // Next, update configuration in database
   m_dbRequester->updateRule(ruleData);

   // Update script file
   m_scriptFactory->updateScriptFile(ruleData, scriptCode);

   // Restart rule
   startRule(ruleData->Id());
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

} // namespace automation	
	
	