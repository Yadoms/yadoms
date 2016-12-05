#pragma once
#include "IRuleManager.h"
#include "IRule.h"
#include "IRuleStateHandler.h"
#include "script/IManager.h"
#include "../communication/ISendMessageAsync.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IRecipientRequester.h"
#include "../dataAccessLayer/IConfigurationManager.h"
#include "../dataAccessLayer/IEventLogger.h"
#include "../dataAccessLayer/IKeywordManager.h"
#include <IPathProvider.h>
#include <shared/event/EventHandler.hpp>
#include <shared/ILocation.h>

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules manager
   //-----------------------------------------------------
   class CRuleManager : public IRuleManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] pathProvider  Yadoms paths provider
      ///\param[in] dbRequester  Database requester
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\param[in] dbDeviceRequester  Database device requester
      ///\param[in] keywordAccessLayer  Database keyword access layer
      ///\param[in] dbRecipientRequester  Database recipient requester
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] eventLogger  Main event logger
      ///\param[in] location  The location provider
      //-----------------------------------------------------
      CRuleManager(const IPathProvider& pathProvider,
                   boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                   boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                   boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                   boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                   boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                   boost::shared_ptr<shared::ILocation> location);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleManager();

      // IRuleManager Implementation
      void stop() override;
      std::vector<std::string> getAvailableInterpreters() override;
      std::vector<boost::shared_ptr<database::entities::CRule>> getRules() const override;
      int createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code) override;
      boost::shared_ptr<database::entities::CRule> getRule(int id) const override;
      std::string getRuleCode(int id) const override;
      std::string getRuleLog(int id) const override;
      std::string getRuleTemplateCode(const std::string& interpreterName) const override;
      void updateRule(boost::shared_ptr<const database::entities::CRule> ruleData) override;
      void updateRuleCode(int id, const std::string& code) override;
      void deleteRule(int id) override;
      void startAllRulesMatchingInterpreter(const std::string& interpreterName) override;
      void stopAllRulesMatchingInterpreter(const std::string& interpreterName) override;
      void deleteAllRulesMatchingInterpreter(const std::string& interpreterName) override;
      void startRule(int ruleId) override;
      bool stopRule(int ruleId) override;
      void stopRuleAndWaitForStopped(int ruleId, const boost::posix_time::time_duration& timeout = boost::posix_time::seconds(20)) override;
      // [END] IRuleManager Implementation


   protected:

      //-----------------------------------------------------
      ///\brief               Start all rules
      //-----------------------------------------------------
      void startAllRules();

      //-----------------------------------------------------
      ///\brief               Start all specified rules
      ///\param[in] rules     Rules to start
      ///\return              true if all rules were successfully started
      //-----------------------------------------------------
      bool startRules(const std::vector<boost::shared_ptr<database::entities::CRule>>& rules);

      //-----------------------------------------------------
      ///\brief               Check if a rule is started
      ///\param[in] ruleId    The rule ID
      ///\return              true if started
      //-----------------------------------------------------
      bool isRuleStarted(int ruleId) const;

      //-----------------------------------------------------
      ///\brief               Stop all started rules
      //-----------------------------------------------------
      void stopRules();

      //-----------------------------------------------------
      ///\brief               Called when rule is stopped
      ///\param[in] ruleId    The rule ID
      ///\param[in] error     Error associated to event (empty if not error)
      //-----------------------------------------------------
      void onRuleStopped(int ruleId, const std::string& error = std::string());

      //-----------------------------------------------------
      ///\brief               Record rule started in base
      ///\param[in] ruleId    The rule ID
      //-----------------------------------------------------
      void recordRuleStarted(int ruleId) const;

      //-----------------------------------------------------
      ///\brief               Record rule stopped in base
      ///\param[in] ruleId    The rule ID
      ///\param[in] error     Error associated to event (empty if not error)
      //-----------------------------------------------------
      void recordRuleStopped(int ruleId, const std::string& error = std::string()) const;

      //-----------------------------------------------------
      ///\brief               Method of the thread managing rule asynchronous events
      //-----------------------------------------------------
      void ruleEventsThreadDoWork();

   private:
      //-----------------------------------------------------
      ///\brief               The rule data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IRuleRequester> m_ruleRequester;

      //-----------------------------------------------------
      ///\brief               Event handler to manage events on all rules
      //-----------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_ruleEventHandler;

      //-----------------------------------------------------
      ///\brief               The script manager
      //-----------------------------------------------------
      boost::shared_ptr<script::IManager> m_scriptManager;

      //-----------------------------------------------------
      ///\brief               The rule state handler
      //-----------------------------------------------------
      boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;

      //-----------------------------------------------------
      ///\brief               Flag indicating that Yadoms is being shutdown, so don't record rules stop in database
      //-----------------------------------------------------
      bool m_yadomsShutdown;

      //-----------------------------------------------------
      ///\brief               Thread managing rule asynchronous events
      //-----------------------------------------------------
      boost::shared_ptr<boost::thread> m_ruleEventsThread;

      //-----------------------------------------------------
      ///\brief               The started rules list (and its mutex)
      //-----------------------------------------------------
      std::map<int, boost::shared_ptr<IRule>> m_startedRules;
      mutable boost::recursive_mutex m_startedRulesMutex;

      //-----------------------------------------------------
      ///\brief               The handlers to notify when a rule stop (potentially several handlers for one rule)
      //-----------------------------------------------------
      mutable boost::recursive_mutex m_ruleStopNotifiersMutex;
      std::map<int, std::set<boost::shared_ptr<shared::event::CEventHandler>>> m_ruleStopNotifiers;
   };
} // namespace automation	


