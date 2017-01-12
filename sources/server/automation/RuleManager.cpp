#include "stdafx.h"
#include "RuleManager.h"
#include "database/IRuleRequester.h"
#include "interpreter/Manager.h"
#include "Rule.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>
#include "RuleException.hpp"
#include "script/GeneralInfo.h"
#include "script/Properties.h"

namespace automation
{
   CRuleManager::CRuleManager(const IPathProvider& pathProvider,
                              boost::shared_ptr<database::IDataProvider> dataProvider,
                              boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                              boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                              boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                              boost::shared_ptr<shared::ILocation> location)
      : m_pluginGateway(pluginGateway),
        m_dbAcquisitionRequester(dataProvider->getAcquisitionRequester()),
        m_dbDeviceRequester(dataProvider->getDeviceRequester()),
        m_keywordAccessLayer(keywordAccessLayer),
        m_dbRecipientRequester(dataProvider->getRecipientRequester()),
        m_eventLogger(eventLogger),
        m_generalInfo(boost::make_shared<script::CGeneralInfo>(location)),
        m_ruleRequester(dataProvider->getRuleRequester()),
        m_ruleEventHandler(boost::make_shared<shared::event::CEventHandler>()),
        m_interpreterManager(boost::make_shared<interpreter::CManager>(pathProvider)),
        m_yadomsShutdown(false)
   {
      m_interpreterManager->setOnScriptStoppedFct(
         [&](int scriptInstanceId, const std::string& error)
         {
            onRuleStopped(scriptInstanceId,
                          error);
         });
   }

   CRuleManager::~CRuleManager()
   {
      if (!m_yadomsShutdown)
         CRuleManager::stop();
   }

   void CRuleManager::start()
   {
      startAllRules();
   }

   void CRuleManager::stop()
   {
      m_yadomsShutdown = true;
      stopRules();
   }

   void CRuleManager::startAllRules()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
      if (!m_startedRules.empty())
         throw shared::exception::CException("Some rules are already started, are you sure that manager was successfuly stopped ?");

      if (!startRules(m_ruleRequester->getRules()))
      YADOMS_LOG(error) << "One or more automation rules failed to start, check automation rules page for details";
   }

   bool CRuleManager::startRules(const std::vector<boost::shared_ptr<database::entities::CRule>>& rules)
   {
      auto allRulesStarted = true;
      for (auto rule = rules.begin();
           rule != rules.end();
           ++rule)
      {
         try
         {
            // Start only autoStarted rules if not in error state
            if ((*rule)->AutoStart() && (*rule)->State() != database::entities::ERuleState::kError)
               startRule((*rule)->Id);
         }
         catch (CRuleException&)
         {
            YADOMS_LOG(error) << "Unable to start rule " << (*rule)->Name() << ", skipped";
            allRulesStarted = false;
         }
      }

      return allRulesStarted;
   }

   std::vector<std::string> CRuleManager::getAvailableInterpreters()
   {
      return m_interpreterManager->getAvailableInterpreters();
   }

   void CRuleManager::startRule(int ruleId)
   {
      try
      {
         auto ruleData = getRule(ruleId);

         if (isRuleStarted(ruleData->Id()))
            return; // Rule already started

         recordRuleStarted(ruleId);

         YADOMS_LOG(information) << "Start rule #" << ruleId;

         boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
         auto newRule(boost::make_shared<CRule>(ruleData,
                                                m_interpreterManager,
                                                m_pluginGateway,
                                                m_dbAcquisitionRequester,
                                                m_dbDeviceRequester,
                                                m_keywordAccessLayer,
                                                m_dbRecipientRequester,
                                                m_generalInfo));
         m_startedRules[ruleId] = newRule;
      }
      catch (shared::exception::CEmptyResult& e)
      {
         const auto& error((boost::format("Invalid rule %1%, element not found in database : %2%") % ruleId % e.what()).str());
         recordRuleStopped(ruleId, error);
         throw CRuleException(error);
      }
      catch (shared::exception::CInvalidParameter& e)
      {
         const auto& error((boost::format("Invalid rule %1% configuration, invalid parameter : %2%") % ruleId % e.what()).str());
         recordRuleStopped(ruleId, error);
         throw CRuleException(error);
      }
      catch (shared::exception::COutOfRange& e)
      {
         const auto& error((boost::format("Invalid rule %1% configuration, out of range : %2%") % ruleId % e.what()).str());
         recordRuleStopped(ruleId, error);
         throw CRuleException(error);
      }
   }

   void CRuleManager::stopRules()
   {
      while (true)
      {
         int ruleIdToStop;
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
            if (m_startedRules.empty())
               return;
            ruleIdToStop = m_startedRules.begin()->first;
         }

         stopRuleAndWaitForStopped(ruleIdToStop);
      }
   }

   bool CRuleManager::stopRule(int ruleId)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
      auto rule = m_startedRules.find(ruleId);

      if (rule == m_startedRules.end())
         return false;

      rule->second->requestStop();
      return true;
   }

   void CRuleManager::stopRuleAndWaitForStopped(int ruleId,
                                                const boost::posix_time::time_duration& timeout)
   {
      auto waitForStoppedRuleHandler(boost::make_shared<shared::event::CEventHandler>());
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_ruleStopNotifiersMutex);
         m_ruleStopNotifiers[ruleId].insert(waitForStoppedRuleHandler);
      }

      if (isRuleStarted(ruleId))
      {
         // Stop the rule
         if (stopRule(ruleId))
         {
            // Wait for rule stopped
            if (waitForStoppedRuleHandler->waitForEvents(timeout) == shared::event::kTimeout)
               throw CRuleException("Unable to stop rule");
         }
      }

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_ruleStopNotifiersMutex);
         m_ruleStopNotifiers[ruleId].erase(waitForStoppedRuleHandler);
      }
   }

   void CRuleManager::onRuleStopped(int ruleId,
                                    const std::string& error)
   {
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
         auto rule = m_startedRules.find(ruleId);

         if (rule == m_startedRules.end())
            return;

         m_startedRules.erase(rule);
      }

      if (!m_yadomsShutdown)
         recordRuleStopped(ruleId, error);

      {
         // Notify all handlers for this rule
         boost::lock_guard<boost::recursive_mutex> lock(m_ruleStopNotifiersMutex);
         auto itEventHandlerSetToNotify = m_ruleStopNotifiers.find(ruleId);
         if (itEventHandlerSetToNotify != m_ruleStopNotifiers.end())
            for (const auto& itHandler : itEventHandlerSetToNotify->second)
               itHandler->postEvent(shared::event::kUserFirstId);
      }
   }

   bool CRuleManager::isRuleStarted(int ruleId) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
      return m_startedRules.find(ruleId) != m_startedRules.end();
   }

   std::vector<boost::shared_ptr<database::entities::CRule>> CRuleManager::getRules() const
   {
      return m_ruleRequester->getRules();
   }

   int CRuleManager::createRule(boost::shared_ptr<const database::entities::CRule> ruleData,
                                const std::string& code)
   {
      // Add rule in database
      auto ruleId = m_ruleRequester->addRule(ruleData);

      // Create script file
      auto ruleProperties(boost::make_shared<script::CProperties>(m_ruleRequester->getRule(ruleId)));
      m_interpreterManager->updateScriptFile(ruleProperties->interpreterName(),
                                             ruleProperties->scriptPath(),
                                             code);

      // Start the rule
      startRule(ruleId);

      return ruleId;
   }

   boost::shared_ptr<database::entities::CRule> CRuleManager::getRule(int id) const
   {
      return m_ruleRequester->getRule(id);
   }

   std::string CRuleManager::getRuleCode(int id) const
   {
      try
      {
         auto ruleProperties(boost::make_shared<script::CProperties>(m_ruleRequester->getRule(id)));
         return m_interpreterManager->getScriptFile(ruleProperties->interpreterName(),
                                                    ruleProperties->scriptPath());
      }
      catch (shared::exception::CEmptyResult& e)
      {
         YADOMS_LOG(error) << "Unable to get rule code : " << e.what();
         return std::string();
      }
   }

   std::string CRuleManager::getRuleLog(int id) const
   {
      try
      {
         return m_interpreterManager->getScriptLogFile(id);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         YADOMS_LOG(error) << "Unable to get rule log : " << e.what();
         return std::string();
      }
      catch (shared::exception::CInvalidParameter& e)
      {
         YADOMS_LOG(error) << "Unable to get rule log : " << e.what();
         return std::string();
      }
   }

   std::string CRuleManager::getRuleTemplateCode(const std::string& interpreterName) const
   {
      try
      {
         return m_interpreterManager->getScriptTemplateFile(interpreterName);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         YADOMS_LOG(error) << "Unable to get rule template code : " << e.what();
         return std::string();
      }
   }

   void CRuleManager::updateRule(boost::shared_ptr<const database::entities::CRule> ruleData)
   {
      // Check for supported modifications
      if (!ruleData->Id.isDefined())
         throw shared::exception::CException("Update rule : rule ID was not provided");

      m_ruleRequester->updateRule(ruleData);
   }

   void CRuleManager::updateRuleCode(int id,
                                     const std::string& code)
   {
      // If rule was started, must be stopped to update its configuration
      auto ruleWasStarted = isRuleStarted(id);
      if (ruleWasStarted)
         stopRuleAndWaitForStopped(id);

      // Update script file
      auto ruleProperties(boost::make_shared<script::CProperties>(m_ruleRequester->getRule(id)));
      m_interpreterManager->updateScriptFile(ruleProperties->interpreterName(),
                                             ruleProperties->scriptPath(),
                                             code);

      // Restart rule
      if (ruleWasStarted)
         startRule(id);
   }

   void CRuleManager::deleteRule(int id)
   {
      try
      {
         auto ruleData(m_ruleRequester->getRule(id));

         // Stop the rule
         stopRuleAndWaitForStopped(id);

         // Remove in database
         m_ruleRequester->deleteRule(id);

         // Remove script file
         auto ruleProperties(boost::make_shared<script::CProperties>(ruleData));
         m_interpreterManager->deleteScriptFile(ruleProperties->interpreterName(),
                                                ruleProperties->scriptPath());
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(error) << "Unable to delete rule (" << id << ") : " << e.what();
         throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(id));
      }
   }

   void CRuleManager::startAllRulesMatchingInterpreter(const std::string& interpreterName)
   {
      // Start all rules associated with this interpreter (and start-able)
      if (!startRules(m_ruleRequester->getRules(interpreterName)))
      YADOMS_LOG(error) << "One or more automation rules failed to start, check automation rules page for details";
   }

   void CRuleManager::stopAllRulesMatchingInterpreter(const std::string& interpreterName)
   {
      // First, stop all running rules associated with this interpreter
      auto interpreterRules = m_ruleRequester->getRules(interpreterName);
      for (auto interpreterRule = interpreterRules.begin();
           interpreterRule != interpreterRules.end();
           ++interpreterRule)
      {
         if (isRuleStarted((*interpreterRule)->Id()))
            stopRuleAndWaitForStopped((*interpreterRule)->Id());
      }

      // We can unload the interpreter as it is not used anymore (will be automaticaly re-loaded when needed by a rule)
      m_interpreterManager->unloadInterpreter(interpreterName);
   }

   void CRuleManager::deleteAllRulesMatchingInterpreter(const std::string& interpreterName)
   {
      auto interpreterRules = m_ruleRequester->getRules(interpreterName);
      for (auto interpreterRule = interpreterRules.begin();
           interpreterRule != interpreterRules.end();
           ++interpreterRule)
         deleteRule((*interpreterRule)->Id());

      // We can unload the interpreter as it is not used anymore (will be automaticaly re-loaded when needed by a rule)
      m_interpreterManager->unloadInterpreter(interpreterName);
   }

   void CRuleManager::recordRuleStarted(int ruleId) const
   {
      auto ruleData(boost::make_shared<database::entities::CRule>());
      ruleData->Id = ruleId;
      ruleData->State = database::entities::ERuleState::kRunning;
      ruleData->StartDate = shared::currentTime::Provider().now();
      ruleData->ErrorMessage = std::string();
      m_ruleRequester->updateRule(ruleData);
   }

   void CRuleManager::recordRuleStopped(int ruleId,
                                        const std::string& error) const
   {
      if (!error.empty())
         YADOMS_LOG(error) << error;

      auto ruleData(boost::make_shared<database::entities::CRule>());
      ruleData->Id = ruleId;
      ruleData->State = error.empty() ? database::entities::ERuleState::kStopped : database::entities::ERuleState::kError;
      ruleData->StopDate = shared::currentTime::Provider().now();
      ruleData->ErrorMessage = error;
      m_ruleRequester->updateRule(ruleData);

      if (!error.empty())
         m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed,
                                 m_ruleRequester->getRule(ruleId)->Name(), error);
   }
} // namespace automation	


