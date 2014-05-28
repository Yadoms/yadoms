#pragma once
#include "database/IDataProvider.h"
#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "command/DeviceCommand.h"
#include "ISendMessageEventHandler.h"
#include <shared/xpl/XplService.h>


namespace communication {


   //----------------------------------------------
   ///\brief Class used to communicate with plugins
   //----------------------------------------------
   class CPluginGateway : public shared::CThreadBase, public shared::event::CEventHandler, public ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief Constructor
      //----------------------------------------------
      CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider);

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CPluginGateway();

      //--------------------------------------------------------------
      /// \brief			Start the gateway, blocking until gateway is fully started
      //--------------------------------------------------------------
      virtual void start();

   private:
      //----------------------------------------------
      ///\brief Enumeration of events used by this class
      //----------------------------------------------
      enum
      {
         kXplMessageReceived = shared::event::kUserFirstId,
         kXplSendMessage
      };

   public:
      // ISendMessageAsync Implementation
      void sendCommandAsync(command::CDeviceCommand & message);
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
      ///\brief  The event handler used to wait for full gateway start
      //----------------------------------------------
      shared::event::CEventHandler m_StartEventHandler;
   };

} //namespace communication
