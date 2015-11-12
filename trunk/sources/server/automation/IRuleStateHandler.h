#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules state management interface
   //-----------------------------------------------------
   class IRuleStateHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRuleStateHandler() {}

      //-----------------------------------------------------
      ///\brief               Signal a normal stop on a rule
      ///\param[in] ruleId    The rule ID
      //-----------------------------------------------------
      virtual void signalNormalRuleStop(int ruleId) = 0;

      //-----------------------------------------------------
      ///\brief               Signal an error on a rule
      ///\param[in] ruleId    The rule ID
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalRuleError(int ruleId, const std::string& error) = 0;

      //-----------------------------------------------------
      ///\brief               Signal error on starting rules
      ///\param[in] ruleId    The rule ID
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalRulesStartError(int ruleId, const std::string& error) = 0;
   };
	
} // namespace automation	
	
	