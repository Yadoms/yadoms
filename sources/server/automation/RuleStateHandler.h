#pragma once
#include "IRuleStateHandler.h"
#include "database/IRuleRequester.h"
#include "dataAccessLayer/IEventLogger.h"
#include <shared/shared/event/EventHandler.hpp>
#include "ManagerEvent.h"

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
      virtual void signalNormalRuleStop(int ruleId);
      virtual void signalRuleError(int ruleId, const std::string& error);
      virtual void signalRulesStartError(const std::string& error);
      // [END] IRuleStateHandler Implementation

      //-----------------------------------------------------
      ///\brief               Post event to supervisor
      ///\param[in] eventType Event type to post to supervisor
      ///\param[in] ruleId    The rule id
      ///\param[in] error     The error label, associated to event (empty if no error)
      //-----------------------------------------------------
      void postToSupervisor(const CManagerEvent::ESubEventType& eventType, int ruleId, const std::string& error = std::string()) const;


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
	
	