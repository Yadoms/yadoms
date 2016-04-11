#pragma once
#include <shared/process/IEndOfProcessObserver.h>
#include <server/dataAccessLayer/IEventLogger.h>
#include <server/database/IPluginRequester.h>
#include <shared/event/EventHandler.hpp>
#include "IInstanceStartErrorObserver.h"

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The instance error handler
   //-----------------------------------------------------
   class CInstanceStateHandler : public IInstanceStartErrorObserver, public shared::process::IEndOfProcessObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] pluginRequester          Database requester
      ///\param[in] eventLogger              Main event logger
      ///\param[in] managerEventHandler      The manager event handler
      ///\param[in] instanceId               The plugin instance ID
      ///\param[in] normalStopEventId        Id of the normal instance stop event
      ///\param[in] abnormalStopEventId      Id of the abnormal instance stop event
      //-----------------------------------------------------
      CInstanceStateHandler(boost::shared_ptr<database::IPluginRequester> pluginRequester,
                            boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                            boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                            int instanceId,
                            int normalStopEventId,
                            int abnormalStopEventId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CInstanceStateHandler();

      // IInstanceStartErrorObserver Implementation
      void signalStartError(const std::string& error) override;
      // [END] IInstanceStartErrorObserver Implementation

      // process::IEndOfProcessObserver Implementation
      void onEndOfProcess(int returnCode, const std::string& error) override;
      // [END] process::IEndOfProcessObserver Implementation


   protected:
      void signalNormalStop() const;
      void signalError(const std::string& error);


   private:
      //-----------------------------------------------------
      ///\brief               The rule data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IPluginRequester> m_pluginRequester;

      //-----------------------------------------------------
      ///\brief               The event logger
      //-----------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;

      //--------------------------------------------------------------
      /// \brief			The plugin manager event handler
      //--------------------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_managerEventHandler;

      //-----------------------------------------------------
      ///\brief               The instance ID
      //-----------------------------------------------------
      int m_instanceId;

      //-----------------------------------------------------
      ///\brief               Stop event IDs
      //-----------------------------------------------------
      int m_normalStopEventId;
      int m_abnormalStopEventId;
   };
	
} // namespace pluginSystem
	
	