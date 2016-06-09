#include "stdafx.h"
#include "RuleManager.h"
#include "RuleStateHandler.h"
#include "database/IRuleRequester.h"
#include "Rule.h"
#include "script/Manager.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>
#include "RuleException.hpp"

namespace automation
{
   CRuleManager::CRuleManager(const IPathProvider& pathProvider,
                              boost::shared_ptr<database::IRuleRequester> dbRequester,
                              boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                              boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                              boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                              boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                              boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                              boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                              boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger)
      : m_ruleRequester(dbRequester),
        m_ruleEventHandler(boost::make_shared<shared::event::CEventHandler>()),
        m_scriptManager(boost::make_shared<script::CManager>(pathProvider,
                                                             pluginGateway,
                                                             configurationManager,
                                                             dbAcquisitionRequester,
                                                             dbDeviceRequester,
                                                             keywordAccessLayer,
                                                             dbRecipientRequester)),
        m_ruleStateHandler(boost::make_shared<CRuleStateHandler>(dbRequester,
                                                                 eventLogger,
                                                                 m_ruleEventHandler)),
        m_yadomsShutdown(false),
        m_ruleEventsThread(boost::make_shared<boost::thread>(boost::bind(&CRuleManager::ruleEventsThreadDoWork, this)))
   {
      startAllRules();
   }

   CRuleManager::~CRuleManager()
   {
      if (!m_yadomsShutdown)
         CRuleManager::stop();
   }

   void CRuleManager::stop()
   {
      m_yadomsShutdown = true;
      stopRules();

      m_ruleEventsThread->interrupt();
      m_ruleEventsThread->join();
   }

   void CRuleManager::startAllRules()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
      if (!m_startedRules.empty())
         throw shared::exception::CException("Some rules are already started, are you sure that manager was successfuly stopped ?");

      if (!startRules(getRules()))
      YADOMS_LOG(error) << "One or more automation rules failed to start, check automation rules page for details";
   }

   bool CRuleManager::startRules(const std::vector<boost::shared_ptr<database::entities::CRule> >& rules)
   {
      bool allRulesStarted = true;
      for (std::vector<boost::shared_ptr<database::entities::CRule> >::const_iterator it = rules.begin(); it != rules.end(); ++it)
      {
         try
         {
            // Start only autoStarted rules if not in error state
            if ((*it)->AutoStart() && (*it)->State() != database::entities::ERuleState::kError)
               startRule((*it)->Id);
         }
         catch (CRuleException&)
         {
            YADOMS_LOG(error) << "Unable to start rule " << (*it)->Name() << ", skipped";
            allRulesStarted = false;
         }
      }

      return allRulesStarted;
   }

   std::vector<std::string> CRuleManager::getAvailableInterpreters()
   {
      return m_scriptManager->getAvailableInterpreters();
   }

   void CRuleManager::startRule(int ruleId)
   {
      try
      {
         boost::shared_ptr<const database::entities::CRule> ruleData = getRule(ruleId);

         if (isRuleStarted(ruleData->Id()))
            return; // Rule already started

         recordRuleStarted(ruleId);

         YADOMS_LOG(information) << "Start rule #" << ruleId;

         boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
         auto newRule(boost::make_shared<CRule>(ruleData, m_scriptManager, m_ruleStateHandler));
         m_startedRules[ruleId] = newRule;
      }
      catch (shared::exception::CEmptyResult& e)
      {
         const std::string& error((boost::format("Invalid rule %1%, element not found in database : %2%") % ruleId % e.what()).str());
         m_ruleStateHandler->signalRuleError(ruleId, error);
         throw CRuleException(error);
      }
      catch (shared::exception::CInvalidParameter& e)
      {
         const std::string& error((boost::format("Invalid rule %1% configuration, invalid parameter : %2%") % ruleId % e.what()).str());
         m_ruleStateHandler->signalRuleError(ruleId, error);
         throw CRuleException(error);
      }
      catch (shared::exception::COutOfRange& e)
      {
         const std::string& error((boost::format("Invalid rule %1% configuration, out of range : %2%") % ruleId % e.what()).str());
         m_ruleStateHandler->signalRuleError(ruleId, error);
         throw CRuleException(error);
      }
   }

   void CRuleManager::stopRules()
   {
      while (!m_startedRules.empty())
         stopRuleAndWaitForStopped(m_startedRules.begin()->first);
   }

   void CRuleManager::stopRule(int ruleId)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
      std::map<int, boost::shared_ptr<IRule> >::iterator rule = m_startedRules.find(ruleId);

      if (rule == m_startedRules.end())
         return;

      rule->second->requestStop();
   }

   void CRuleManager::stopRuleAndWaitForStopped(int ruleId)
   {
      boost::shared_ptr<shared::event::CEventHandler> waitForStoppedRuleHandler(boost::make_shared<shared::event::CEventHandler>());
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_ruleStopNotifiersMutex);
         m_ruleStopNotifiers[ruleId].insert(waitForStoppedRuleHandler);
      }

      if (isRuleStarted(ruleId))
      {
         // Stop the rule
         stopRule(ruleId);

         // Wait for rule stopped
         if (waitForStoppedRuleHandler->waitForEvents(boost::posix_time::seconds(10)) == shared::event::kTimeout)
            throw CRuleException("Unable to stop rule");
      }

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_ruleStopNotifiersMutex);
         m_ruleStopNotifiers[ruleId].erase(waitForStoppedRuleHandler);
      }
   }

   void CRuleManager::onRuleStopped(int ruleId, const std::string& error)
   {
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
         std::map<int, boost::shared_ptr<IRule> >::iterator rule = m_startedRules.find(ruleId);

         if (rule == m_startedRules.end())
            return;

         m_startedRules.erase(rule);

         if (!m_yadomsShutdown)
            recordRuleStopped(ruleId, error);
      }

      {
         // Notify all handlers for this rule
         boost::lock_guard<boost::recursive_mutex> lock(m_ruleStopNotifiersMutex);
         std::map<int, std::set<boost::shared_ptr<shared::event::CEventHandler> > >::const_iterator itEventHandlerSetToNotify = m_ruleStopNotifiers.find(ruleId);
         if (itEventHandlerSetToNotify != m_ruleStopNotifiers.end())
            for (std::set<boost::shared_ptr<shared::event::CEventHandler> >::const_iterator itHandler = itEventHandlerSetToNotify->second.begin(); itHandler != itEventHandlerSetToNotify->second.end(); ++itHandler)
               (*itHandler)->postEvent(shared::event::kUserFirstId);
      }
   }

   bool CRuleManager::isRuleStarted(int ruleId)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_startedRulesMutex);
      return m_startedRules.find(ruleId) != m_startedRules.end();
   }

   std::vector<boost::shared_ptr<database::entities::CRule> > CRuleManager::getRules() const
   {
      return m_ruleRequester->getRules();
   }

   int CRuleManager::createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code)
   {
      // Add rule in database
      int ruleId = m_ruleRequester->addRule(ruleData);

      // Get the created rule from the id
      boost::shared_ptr<database::entities::CRule> updatedRuleData = m_ruleRequester->getRule(ruleId);

      // Create script file
      m_scriptManager->updateScriptFile(updatedRuleData, code);

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
         boost::shared_ptr<database::entities::CRule> ruleData(m_ruleRequester->getRule(id));
         return m_scriptManager->getScriptFile(ruleData);
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
         boost::shared_ptr<database::entities::CRule> ruleData(m_ruleRequester->getRule(id));
         return m_scriptManager->getScriptLogFile(ruleData);
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
         return m_scriptManager->getScriptTemplateFile(interpreterName);
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
         throw new shared::exception::CException("Update rule : rule ID was not provided");

      m_ruleRequester->updateRule(ruleData);
   }

   void CRuleManager::updateRuleCode(int id, const std::string& code)
   {
      // If rule was started, must be stopped to update its configuration
      bool ruleWasStarted = isRuleStarted(id);
      if (ruleWasStarted)
         stopRuleAndWaitForStopped(id);

      // Update script file
      boost::shared_ptr<database::entities::CRule> ruleData(m_ruleRequester->getRule(id));
      m_scriptManager->updateScriptFile(ruleData, code);

      // Restart rule
      if (ruleWasStarted)
         startRule(id);
   }

   void CRuleManager::deleteRule(int id)
   {
      try
      {
         boost::shared_ptr<database::entities::CRule> ruleData(m_ruleRequester->getRule(id));

         // Stop the rule
         stopRuleAndWaitForStopped(id);

         // Remove in database
         m_ruleRequester->deleteRule(id);

         // Remove script file
         m_scriptManager->deleteScriptFile(ruleData);
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(error) << "Unable to delete rule (" << id << ") : " << e.what();
         throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(id));
      }
   }

   void CRuleManager::ruleEventsThreadDoWork()
   {
      try
      {
         while (true)
         {
            switch (m_ruleEventHandler->waitForEvents())
            {
            case CRuleStateHandler::kRuleAbnormalStopped:
               {
                  // The rule has stopped in a non-conventional way (probably crashed)
                  std::pair<int, std::string> data = m_ruleEventHandler->getEventData<std::pair<int, std::string> >();
                  onRuleStopped(data.first, data.second);
                  break;
               }
            case CRuleStateHandler::kRuleStopped:
               {
                  onRuleStopped(m_ruleEventHandler->getEventData<int>());
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
      }
      catch (boost::thread_interrupted&)
      {
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
      std::vector<boost::shared_ptr<database::entities::CRule> > interpreterRules = m_ruleRequester->getRules(interpreterName);
      for (std::vector<boost::shared_ptr<database::entities::CRule> >::const_iterator interpreterRule = interpreterRules.begin(); interpreterRule != interpreterRules.end(); ++interpreterRule)
      {
         if (isRuleStarted((*interpreterRule)->Id()))
            stopRuleAndWaitForStopped((*interpreterRule)->Id());
      }

      // We can unload the interpreter as it is not used anymore (will be automaticaly re-loaded when needed by a rule)
      m_scriptManager->unloadInterpreter(interpreterName);
   }

   void CRuleManager::deleteAllRulesMatchingInterpreter(const std::string& interpreterName)
   {
      std::vector<boost::shared_ptr<database::entities::CRule> > interpreterRules = m_ruleRequester->getRules(interpreterName);
      for (std::vector<boost::shared_ptr<database::entities::CRule> >::const_iterator interpreterRule = interpreterRules.begin(); interpreterRule != interpreterRules.end(); ++interpreterRule)
         deleteRule((*interpreterRule)->Id());

      // We can unload the interpreter as it is not used anymore (will be automaticaly re-loaded when needed by a rule)
      m_scriptManager->unloadInterpreter(interpreterName);
   }

   void CRuleManager::recordRuleStarted(int ruleId) const
   {
      boost::shared_ptr<database::entities::CRule> ruleData(boost::make_shared<database::entities::CRule>());
      ruleData->Id = ruleId;
      ruleData->State = database::entities::ERuleState::kRunning;
      ruleData->StartDate = shared::currentTime::Provider::now();
      ruleData->ErrorMessage = std::string();
      m_ruleRequester->updateRule(ruleData);
   }

   void CRuleManager::recordRuleStopped(int ruleId, const std::string& error) const
   {
      boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
      ruleData->Id = ruleId;
      ruleData->State = error.empty() ? database::entities::ERuleState::kStopped : database::entities::ERuleState::kError;
      ruleData->StopDate = shared::currentTime::Provider::now();
      ruleData->ErrorMessage = error;
      m_ruleRequester->updateRule(ruleData);
   }
} // namespace automation	


