#pragma once
#include "IInstanceStateHandler.h"
#include <server/dataAccessLayer/IEventLogger.h>
#include <server/database/IPluginRequester.h>
#include <shared/event/EventHandler.hpp>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The instance error handler
   //-----------------------------------------------------
   class CInstanceStateHandler : public IInstanceStateHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] pluginRequester  Database requester
      ///\param[in] eventLogger  Main event logger
      ///\param[in] managerEventHandler  the manager event handler
      ///\param[in] instanceId  the plugin instance ID
      //-----------------------------------------------------
      CInstanceStateHandler(boost::shared_ptr<database::IPluginRequester> pluginRequester,
                            boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                            boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                            int instanceId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CInstanceStateHandler();

      // IInstanceStateHandler Implementation
      void signalNormalStop() override;
      void signalError(const std::string& error) override;
      void signalStartError(const std::string& error) override;
      // [END] IInstanceStateHandler Implementation


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
   };
	
} // namespace pluginSystem
	
	