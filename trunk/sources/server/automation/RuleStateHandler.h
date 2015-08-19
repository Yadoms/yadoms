#pragma once
#include "IRuleStateHandler.h"
#include "database/IRuleRequester.h"
#include "dataAccessLayer/IEventLogger.h"
#include <shared/shared/event/EventHandler.hpp>

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules error handler
   //-----------------------------------------------------
   class CRuleStateHandler : public IRuleStateHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] dbRequester  Database requester
      ///\param[in] eventLogger  Main event logger
      ///\param[in] supervisor     the supervisor event handler
      ///\param[in] ruleManagerEventId    The ID to use to send events to supervisor
      //-----------------------------------------------------
      CRuleStateHandler(boost::shared_ptr<database::IRuleRequester> dbRequester,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
         boost::shared_ptr<shared::event::CEventHandler> supervisor, int ruleManagerEventId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleStateHandler();

      // IRuleStateHandler Implementation
      virtual void signalRuleStart(int ruleId);
      virtual void signalNormalRuleStop(int ruleId);
      virtual void signalNormalRuleStopAndDisable(int ruleId);
      virtual void signalRuleError(int ruleId, const std::string& error);
      virtual void signalRulesStartError(const std::string& error);
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
      boost::shared_ptr<shared::event::CEventHandler> m_supervisor;

      //--------------------------------------------------------------
      /// \brief			ID to use to send events to supervisor
      //--------------------------------------------------------------
      const int m_ruleManagerEventId;
   };
	
} // namespace automation	
	
	