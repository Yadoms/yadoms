#pragma once
#include "database/IDataProvider.h"
#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "command/DeviceCommand.h"
#include "pluginSystem/Manager.h"
#include "ISendMessageEventHandler.h"


namespace communication {


   //----------------------------------------------
   ///\brief Class used to communicate with plugins
   //----------------------------------------------
   class CPluginGateway : public shared::CThreadBase, public ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief                     Constructor
      ///\param[in] dataProvider    The main data provider (access to database)
      ///\param[in] pluginManager   The plugin manager
      //----------------------------------------------
      CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<pluginSystem::CManager> pluginManager);

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CPluginGateway();

      //--------------------------------------------------------------
      /// \brief			Start the gateway, blocking until gateway is fully started
      //--------------------------------------------------------------
      virtual void start();

   public:
      // ISendMessageAsync Implementation
      void sendCommandAsync(const command::CDeviceCommand & message);
      // [END] ISendMessageAsync Implementation

   private:
      // CThreadBase Implementation
      void doWork();
      // [END] CThreadBase Implementation

      //----------------------------------------------
      ///\brief Wait full start of gateway
      ///\throw CException if timeout or bad event received
      //----------------------------------------------
      void waitForstarted();

   private:
      //----------------------------------------------
      ///\brief  The data provider
      //----------------------------------------------
      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      //----------------------------------------------
      ///\brief  The plugin manager
      //----------------------------------------------
      boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

      //----------------------------------------------
      ///\brief  The event handler used to wait for full gateway start
      //----------------------------------------------
      shared::event::CEventHandler m_startEventHandler;

      //----------------------------------------------
      ///\brief  The main event handler, used to send/receive data from/to plugins
      //----------------------------------------------
      shared::event::CEventHandler m_mainEventHandler;
   };

} //namespace communication
