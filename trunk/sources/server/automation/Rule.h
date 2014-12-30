#pragma once
#include "IRule.h"
#include <shared/shared/DataContainer.h>
#include "RuleThread.h"
#include "condition/IConditionRoot.h"
#include "condition/IConditionFactory.h"
#include "IActionList.h"
#include "../communication/ISendMessageAsync.h"
#include "../database/sqlite/requesters/Rule.h"
#include "INotificationObserverForRulesManager.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief A rule
   //-----------------------------------------------------
   class CRule : public IRule, public IRuleThreadHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] ruleData   Rule data (ID, name, conditions, actions...)
      ///\param[in] notificationObserver  The notification observer
      ///\param[in] pluginGateway plugin access to do actions on plugins
      ///\param[in] conditionFactory the condition factory
      //-----------------------------------------------------
      CRule(const database::entities::CRule& ruleData,
         boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver,
         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         condition::IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRule();

      // IRule Implementation
      virtual void start();
      virtual void stop();
      // [END] IRule Implementation

   protected:
      // IRuleThreadHandler Implementation
      virtual void doWork();
      // [END] IRuleThreadHandler Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Rule name
      //-----------------------------------------------------
      const std::string m_name;

      //-----------------------------------------------------
      ///\brief               Rule ID
      //-----------------------------------------------------
      const int m_id;

      //-----------------------------------------------------
      ///\brief               Rule thread
      //-----------------------------------------------------
      boost::shared_ptr<CRuleThread> m_thread;

      //-----------------------------------------------------
      ///\brief               The condition to wait
      //-----------------------------------------------------
      boost::shared_ptr<condition::IConditionRoot> m_condition;

      //-----------------------------------------------------
      ///\brief               The actions to do
      //-----------------------------------------------------
      boost::shared_ptr<IActionList> m_actions;

      //-----------------------------------------------------
      ///\brief               The notification observer
      //-----------------------------------------------------
      boost::shared_ptr<INotificationObserverForRulesManager> m_notificationObserver;
   };
	
} // namespace automation	
	
	