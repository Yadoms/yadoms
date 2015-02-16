#include "stdafx.h"
#include "RuleManager.h"
#include "database/IRuleRequester.h"
#include "Rule.h"
#include "condition/ConditionFactory.h"
#include "action/script/Factory.h"
#include "NotificationObserverForRulesManager.h"
#include <shared/exception/EmptyResult.hpp>
#include "RuleException.hpp"

namespace automation
{

CRuleManager::CRuleManager(boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_pluginGateway(pluginGateway),
   m_dbRequester(dbRequester),
   m_conditionFactory(new condition::CConditionFactory(dbAcquisitionRequester)),
   m_notificationCenter(notificationCenter),
   m_scriptFactory(new action::script::CFactory("scriptInterpreters", pluginGateway, notificationCenter, dbAcquisitionRequester))
{        
}

CRuleManager::~CRuleManager()
{        
}

void CRuleManager::start()
{
   BOOST_ASSERT_MSG(m_startedRules.empty(), "Some rules are already started, are you sure that manager was successfuly stopped ?");

   // Register notification observer
   m_notificationObserver.reset(new CNotificationObserverForRulesManager(m_notificationCenter));

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

   // Unregister notification observer
   m_notificationObserver.reset();
}

void CRuleManager::startRule(int ruleId)
{
   try
   {
      boost::shared_ptr<const database::entities::CRule> ruleData = getRule(ruleId);

      if (!ruleData->Enabled())
         return;  // Rule not enabled, don't start

      boost::shared_ptr<IRule> newRule(new CRule(*ruleData, m_notificationObserver, m_pluginGateway, *m_conditionFactory, m_scriptFactory));
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

int CRuleManager::createRule(const database::entities::CRule& data)
{
   int ruleId = m_dbRequester->addRule(data);
   startRule(ruleId);
   return ruleId;   
}

boost::shared_ptr<database::entities::CRule> CRuleManager::getRule(int id) const
{
   return m_dbRequester->getRule(id);
}

void CRuleManager::updateRule(const database::entities::CRule& newData)
{
   // Check for supported modifications
   if (!newData.Id.isDefined())
   {
      BOOST_ASSERT(false); // ID must be provided
      throw new shared::exception::CException("Update rule : rule ID was not provided");
   }

   // If rule was started, must be stopped to update its configuration
   if (m_startedRules.find(newData.Id()) != m_startedRules.end())
      stopRule(newData.Id());

   // Next, update configuration in database
   m_dbRequester->updateRule(newData);

   // Restart rule
   startRule(newData.Id());
}

void CRuleManager::deleteRule(int id)
{
   try
   {
      stopRule(id);
      m_dbRequester->deleteRule(id);
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Unable to delete rule (" << id << ") : " << e.what();
      throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(id));
   }
}

} // namespace automation	
	
	