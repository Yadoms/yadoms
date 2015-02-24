#pragma once
#include "IRuleErrorHandler.h"
#include "database/IRuleRequester.h"
#include "../database/IEventLoggerRequester.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules error handler
   //-----------------------------------------------------
   class CRuleErrorHandler : public IRuleErrorHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] dbRequester  Database requester
      ///\param[in] eventLoggerRequester  Event logger requester
      //-----------------------------------------------------
      CRuleErrorHandler(boost::shared_ptr<database::IRuleRequester> dbRequester, boost::shared_ptr<database::IEventLoggerRequester> eventLoggerRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleErrorHandler();

      // IRuleErrorHandler Implementation
      virtual void signalRuleStart(int ruleId);
      virtual void signalNormalRuleStop(int ruleId);
      virtual void signalRuleError(int ruleId, const std::string& error);
      virtual void signalRulesStartError(const std::string& error);
      // [END] IRuleErrorHandler Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The rule data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IRuleRequester> m_ruleRequester;

      //-----------------------------------------------------
      ///\brief               The event logger
      //-----------------------------------------------------
      boost::shared_ptr<database::IEventLoggerRequester> m_eventLoggerRequester;
   };
	
} // namespace automation	
	
	