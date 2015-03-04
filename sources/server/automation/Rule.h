#pragma once
#include "IRule.h"
#include "RuleThread.h"
#include "script/IFactory.h"
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
      ///\param[in] scriptFactory The script factory
      ///\param[in] ruleStateHandler The rule state handler
      //-----------------------------------------------------
      CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
         boost::shared_ptr<script::IFactory> scriptFactory, boost::shared_ptr<IRuleStateHandler> ruleStateHandler);

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
      ///\brief               Rule data
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CRule> m_ruleData;

      //-----------------------------------------------------
      ///\brief               Rule thread
      //-----------------------------------------------------
      boost::shared_ptr<CRuleThread> m_thread;

      //-----------------------------------------------------
      ///\brief               The script factory
      //-----------------------------------------------------
      boost::shared_ptr<script::IFactory> m_scriptFactory;

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
	
	