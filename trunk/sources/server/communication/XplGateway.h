#pragma once
#include "database/IDataProvider.h"
#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "rules/RulerFactory.h"
#include "command/DeviceCommand.h"
#include "ISendMessageEventHandler.h"
#include <shared/xpl/XplService.h>


namespace communication {


   //----------------------------------------------
   ///\brief Class used to communicate througt Xpl
   //----------------------------------------------
   class CXplGateway : public shared::CThreadBase, public shared::event::CEventHandler, public ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief Constructor
      //----------------------------------------------
      CXplGateway(boost::shared_ptr<database::IDataProvider> dataProvider);

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CXplGateway();

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
      
      //----------------------------------------------
      ///\brief Function handler when receiving XplMessage
      ///\param [in] The xpl message received
      //----------------------------------------------
      void OnXplMessageReceived(shared::xpl::CXplMessage & message);

      //----------------------------------------------
      ///\brief Function handler used to send a command to a device
      ///\param [in] message : The command to send
      ///\param [in] xplService : the xpl service
      //----------------------------------------------
      void OnSendDeviceCommand(command::CDeviceCommand & message, shared::xpl::CXplService & xplService);

   private:
      //----------------------------------------------
      ///\brief  The data provider
      //----------------------------------------------
      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      //----------------------------------------------
      ///\brief  The ruler factory
      //----------------------------------------------
      rules::CRulerFactory m_rulerFactory;

      //----------------------------------------------
      ///\brief  The gateway xpl device id
      //----------------------------------------------
      static std::string m_gateway_device_id;

      //----------------------------------------------
      ///\brief  The event handler used to wait for full gateway start
      //----------------------------------------------
      shared::event::CEventHandler m_StartEventHandler;
   };

} //namespace communication
