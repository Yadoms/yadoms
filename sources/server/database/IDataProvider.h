#pragma once

#include "IPluginRequester.h"
#include "IConfigurationRequester.h"
#include "IDeviceRequester.h"
#include "IKeywordRequester.h"
#include "IPageRequester.h"
#include "IEventLoggerRequester.h"
#include "IWidgetRequester.h"
#include "IPluginEventLoggerRequester.h"
#include "IRuleRequester.h"
#include "IRecipientRequester.h"
#include "IAcquisitionRequester.h"
#include "ITransactionalProvider.h"
#include "IDatabaseRequester.h"

namespace database
{
   class IDataProvider
   {
   public:
      virtual ~IDataProvider()
      {
      }

      //--------------------------------------------------------------
      /// \Brief		Load the database system
      /// \return 	true if database is loaded with success
      /// \throws 		
      //--------------------------------------------------------------
      virtual bool load() = 0;

      //--------------------------------------------------------------
      /// \Brief	Unload the database system
      //--------------------------------------------------------------
      virtual void unload() = 0;

      //--------------------------------------------------------------
      /// \Brief	Stop all maintenance tasks
      //--------------------------------------------------------------
      virtual void stopMaintenanceTasks() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the transactional engine
      /// \return 	the transactional engine (can be NULL if transactional mode is not supported)
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<ITransactionalProvider> getTransactionalEngine() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the plugin requester
      /// \return 	the plugin requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IPluginRequester> getPluginRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the configuration requester
      /// \return 	the configuration requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IConfigurationRequester> getConfigurationRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the device requester
      /// \return 	the device requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IDeviceRequester> getDeviceRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the keyword requester
      /// \return 	the keyword requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IKeywordRequester> getKeywordRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the page requester
      /// \return 	the page requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IPageRequester> getPageRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the widget requester
      /// \return 	the widget requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IWidgetRequester> getWidgetRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the plugin event logger requester
      /// \return 	the plugin event logger requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IPluginEventLoggerRequester> getPluginEventLoggerRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the Acquisition requester
      /// \return 	the Acquisiton requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IAcquisitionRequester> getAcquisitionRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the Event Logger requester
      /// \return 	the Event Logger requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IEventLoggerRequester> getEventLoggerRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the rule requester
      /// \return 	the rule requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IRuleRequester> getRuleRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the Recipient requester
      /// \return 	the Recipient requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IRecipientRequester> getRecipientRequester() = 0;

      //--------------------------------------------------------------
      /// \Brief		get the Database requester
      /// \return 	the Database requester
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<IDatabaseRequester> getDatabaseRequester() = 0;
   };
} //namespace database 


