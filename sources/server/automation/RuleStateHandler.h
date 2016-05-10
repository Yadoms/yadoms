#pragma once
#include "IRuleStateHandler.h"
#include "database/IRuleRequester.h"
#include "dataAccessLayer/IEventLogger.h"
#include <shared/event/EventHandler.hpp>

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules error handler
   //-----------------------------------------------------
   class CRuleStateHandler : public IRuleStateHandler
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Type of rule manager event
      //--------------------------------------------------------------
      enum ERuleEventType
      {
         kRuleAbnormalStopped = 0,      // Rule abnormal stopped
         kRuleStopped
      };

   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] dbRequester  Database requester
      ///\param[in] eventLogger  Main event logger
      ///\param[in] ruleEventHandler  the rule manager event handler
      //-----------------------------------------------------
      CRuleStateHandler(boost::shared_ptr<database::IRuleRequester> dbRequester,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
         boost::shared_ptr<shared::event::CEventHandler> ruleEventHandler);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleStateHandler();

      // IRuleStateHandler Implementation
      virtual void signalNormalRuleStop(int ruleId);
      virtual void signalRuleError(int ruleId, const std::string& error);
      virtual void signalRulesStartError(int ruleId, const std::string& error);
      // [END] IRuleStateHandler Implementation


   private:
      //-----------------------------------------------------
      ///\brief               The rule data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IRuleRequester> m_ruleRequester;

      //-----------------------------------------------------
      ///\brief               The event logger
      //-----------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;

      //--------------------------------------------------------------
      /// \brief			The supervisor event handler
      //--------------------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_ruleEventHandler;
   };
	
} // namespace automation	
	
	