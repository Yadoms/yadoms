#pragma once
#include "IRuleManager.h"
#include "IRule.h"
#include "IRuleStateHandler.h"
#include "script/IManager.h"
#include "../communication/ISendMessageAsync.h"
#include "notification/NotificationCenter.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IRecipientRequester.h"
#include "../dataAccessLayer/IConfigurationManager.h"
#include "../dataAccessLayer/IEventLogger.h"
#include "../IRunningInformation.h"
#include <shared/event/EventHandler.hpp>

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
      ///\param[in] dbRequester  Database requester
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\param[in] dbDeviceRequester  Database device requester
      ///\param[in] dbKeywordRequester  Database keyword requester
      ///\param[in] dbRecipientRequester  Database recipient requester
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] eventLogger  Main event logger
      ///\param[in] supervisor     the supervisor event handler
      ///\param[in] ruleManagerEventId    The ID to use to send events to supervisor
      //-----------------------------------------------------
      CRuleManager(boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
         boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
         boost::shared_ptr<database::IKeywordRequester> dbKeywordRequester,
         boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
         boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger, boost::shared_ptr<shared::event::CEventHandler> supervisor, int ruleManagerEventId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleManager();

      // IRuleManager Implementation
      virtual std::vector<std::string> getAvailableInterpreters();
      virtual std::vector<boost::shared_ptr<database::entities::CRule> > getRules() const;
      virtual int createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code);
      virtual boost::shared_ptr<database::entities::CRule> getRule(int id) const;
      virtual std::string getRuleCode(int id) const;
      virtual std::string getRuleLog(int id) const;
      virtual void updateRule(boost::shared_ptr<const database::entities::CRule> ruleData);
      virtual void updateRuleCode(int id, const std::string& code);
      virtual void deleteRule(int id);
      virtual void restartRule(int id);
      virtual void signalEvent(const CManagerEvent& event);
      virtual void startAllRulesMatchingInterpreter(const std::string & interpreterName);
      virtual void stopAllRulesMatchingInterpreter(const std::string & interpreterName);
      virtual void deleteAllRulesMatchingInterpreter(const std::string & interpreterName);
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
      bool startRules(const std::vector<boost::shared_ptr<database::entities::CRule> >& rules);

      //-----------------------------------------------------
      ///\brief               Start a rule from configured rule data
      ///\param[in] ruleId    The rule ID
      ///\throw CRuleException Error starting rule
      //-----------------------------------------------------
      void startRule(int ruleId);

      //-----------------------------------------------------
      ///\brief               Stop a rule
      ///\param[in] ruleId    The rule ID
      //-----------------------------------------------------
      void stopRule(int ruleId);

      //-----------------------------------------------------
      ///\brief               Check if a rule is started
      ///\param[in] ruleId    The rule ID
      ///\return              true if started
      //-----------------------------------------------------
      bool isRuleStarted(int ruleId);

   private:
      //-----------------------------------------------------
      ///\brief               The rule data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IRuleRequester> m_dbRequester;

      //-----------------------------------------------------
      ///\brief               The script manager
      //-----------------------------------------------------
      boost::shared_ptr<script::IManager> m_scriptManager;

      //-----------------------------------------------------
      ///\brief               The started rules list
      //-----------------------------------------------------
      std::map<int, boost::shared_ptr<IRule> > m_startedRules;

      //-----------------------------------------------------
      ///\brief               The rule state handler
      //-----------------------------------------------------
      boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;
   };
	
} // namespace automation	
	
	