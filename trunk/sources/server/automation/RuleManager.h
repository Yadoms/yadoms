#pragma once
#include "IRuleManager.h"
#include "IRule.h"
#include "script/IFactory.h"
#include "database/IRuleRequester.h"
#include "../communication/ISendMessageAsync.h"
#include <shared/notification/NotificationCenter.h>
#include "database/IAcquisitionRequester.h"

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
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      //-----------------------------------------------------
      CRuleManager(boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleManager();

      // IRuleManager Implementation
      virtual void start();
      virtual void stop();
      virtual std::vector<boost::shared_ptr<database::entities::CRule> > getRules() const;
      virtual int createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code);
      virtual boost::shared_ptr<database::entities::CRule> getRule(int id) const;
      virtual void updateRule(boost::shared_ptr<const database::entities::CRule> ruleData);
      virtual void deleteRule(int id);
      // [END] IRuleManager Implementation

   protected:
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

   private:
      //-----------------------------------------------------
      ///\brief               The rule data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IRuleRequester> m_dbRequester;

      //-----------------------------------------------------
      ///\brief               The script  factory
      //-----------------------------------------------------
      boost::shared_ptr<script::IFactory> m_scriptFactory;

      //-----------------------------------------------------
      ///\brief               The started rules list
      //-----------------------------------------------------
      std::map<int, boost::shared_ptr<IRule> > m_startedRules;
   };
	
} // namespace automation	
	
	