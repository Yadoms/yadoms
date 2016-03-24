#pragma once
#include "IPluginStateHandler.h"
#include "database/IPluginRequester.h"
#include "dataAccessLayer/IEventLogger.h"
#include <shared/event/EventHandler.hpp>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The plugins error handler
   //-----------------------------------------------------
   class CPluginStateHandler : public IPluginStateHandler
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Type of plugin manager event
      //--------------------------------------------------------------
      enum EPluginEventType
      {
         kAbnormalStopped = 0,      // Plugin abnormal stopped
         kStopped
      };

   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] dbRequester  Database requester
      ///\param[in] eventLogger  Main event logger
      ///\param[in] pluginEventHandler  the plugin manager event handler
      //-----------------------------------------------------
      CPluginStateHandler(boost::shared_ptr<database::IPluginRequester> dbRequester,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
         boost::shared_ptr<shared::event::CEventHandler> pluginEventHandler);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CPluginStateHandler();

      // IPluginStateHandler Implementation
      virtual void signalNormalStop(int instanceId);
      virtual void signalError(int instanceId, const std::string& error);
      virtual void signalStartError(int instanceId, const std::string& error);
      // [END] IPluginStateHandler Implementation


   private:
      //-----------------------------------------------------
      ///\brief               The plugin data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IPluginRequester> m_pluginRequester;

      //-----------------------------------------------------
      ///\brief               The event logger
      //-----------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;

      //--------------------------------------------------------------
      /// \brief			The supervisor event handler
      //--------------------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_pluginEventHandler;
   };
	
} // namespace pluginSystem	
	
	