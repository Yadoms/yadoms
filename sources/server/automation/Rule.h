#pragma once
#include "IRule.h"
#include "RuleThread.h"
#include "script/IManager.h"
#include "../database/sqlite/requesters/Rule.h"
#include "IRuleStateHandler.h"

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
      ///\param[in] scriptManager The script manager
      ///\param[in] ruleStateHandler The rule state handler
      //-----------------------------------------------------
      CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
         boost::shared_ptr<script::IManager> scriptManager, boost::shared_ptr<IRuleStateHandler> ruleStateHandler);

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
      ///\brief               Minimum rule duration
      //-----------------------------------------------------
      static const boost::chrono::milliseconds m_MinRuleDuration;

      //-----------------------------------------------------
      ///\brief               Rule data
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CRule> m_ruleData;

      //-----------------------------------------------------
      ///\brief               Rule thread
      //-----------------------------------------------------
      boost::shared_ptr<CRuleThread> m_thread;

      //-----------------------------------------------------
      ///\brief               The script manager
      //-----------------------------------------------------
      boost::shared_ptr<script::IManager> m_scriptManager;

      //-----------------------------------------------------
      ///\brief               The script runner
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IRunner> m_runner;

      //-----------------------------------------------------
      ///\brief               The rule state handler
      //-----------------------------------------------------
      boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;
   };
	
} // namespace automation	
	
	