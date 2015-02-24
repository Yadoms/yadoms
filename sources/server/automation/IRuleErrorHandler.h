#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules error management interface
   //-----------------------------------------------------
   class IRuleErrorHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRuleErrorHandler() {}

      //-----------------------------------------------------
      ///\brief               Signal the start of the rule
      ///\param[in] ruleId    The rule ID
      //-----------------------------------------------------
      virtual void signalRuleStart(int ruleId) = 0;

      //-----------------------------------------------------
      ///\brief               Signal an error on a rule
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
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalRulesStartError(const std::string& error) = 0;
   };
	
} // namespace automation	
	
	