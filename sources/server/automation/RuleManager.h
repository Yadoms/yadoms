#pragma once
#include "IRuleManager.h"
#include "IRule.h"
#include "../communication/ISendMessageAsync.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IRecipientRequester.h"
#include "../dataAccessLayer/IKeywordManager.h"
#include <shared/event/EventHandler.hpp>
#include <shared/ILocation.h>
#include "interpreter/IManager.h"
#include "database/IRuleRequester.h"
#include "script/IGeneralInfo.h"
#include "database/IDataProvider.h"
#include "dataAccessLayer/IEventLogger.h"
#include "dateTime/ITimeZoneProvider.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules manager
   //-----------------------------------------------------
   class CRuleManager : public IRuleManager
   {
   public:
      CRuleManager(boost::shared_ptr<interpreter::IManager> interpreterManager,
                   boost::shared_ptr<database::IDataProvider> dataProvider,
                   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                   boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                   boost::shared_ptr<shared::ILocation> location,
                   boost::shared_ptr<dateTime::ITimeZoneProvider> timezoneProvider);
      virtual ~CRuleManager();

      // IRuleManager Implementation
      void start() override;
      void stop() override;
      std::vector<std::string> getLoadedInterpreters() override;
      std::vector<boost::shared_ptr<database::entities::CRule>> getRules() const override;
      int createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code) override;
      boost::shared_ptr<database::entities::CRule> getRule(int id) const override;
      std::string getRuleCode(int id) const override;
      std::string getRuleLog(int id) const override;
      void deleteRuleLog(int id) const override;
      std::string getRuleTemplateCode(const std::string& interpreterName) const override;
      void updateRule(boost::shared_ptr<const database::entities::CRule> ruleData) override;
      void updateRuleCode(int id, const std::string& code) override;
      void deleteRule(int id) override;
      int duplicateRule(int idToDuplicate, const std::string & newName) override;
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
      void onRuleStopped(int ruleId,
                         const std::string& error = std::string());

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
      void recordRuleStopped(int ruleId,
                             const std::string& error = std::string()) const;

   private:
      boost::shared_ptr<interpreter::IManager> m_interpreterManager;
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;
      boost::shared_ptr<database::IDeviceRequester> m_dbDeviceRequester;
      boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordAccessLayer;
      boost::shared_ptr<database::IRecipientRequester> m_dbRecipientRequester;
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;
      boost::shared_ptr<script::IGeneralInfo> m_generalInfo;

      boost::shared_ptr<database::IRuleRequester> m_ruleRequester;
      boost::shared_ptr<shared::event::CEventHandler> m_ruleEventHandler;
      bool m_yadomsShutdown;

      std::map<int, boost::shared_ptr<IRule>> m_startedRules;
      mutable boost::recursive_mutex m_startedRulesMutex;

      mutable boost::recursive_mutex m_ruleStopNotifiersMutex;
      std::map<int, std::set<boost::shared_ptr<shared::event::CEventHandler>>> m_ruleStopNotifiers;
   };
} // namespace automation	


