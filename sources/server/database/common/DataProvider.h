#pragma once

#include "../IDataProvider.h"
#include "../IDataBackup.h"
#include "../IDatabaseRequester.h"
#include "requesters/Plugin.h"
#include "requesters/Configuration.h"
#include "requesters/Device.h"
#include "requesters/Keyword.h"
#include "requesters/Page.h"
#include "requesters/Rule.h"
#include "requesters/Widget.h"
#include "requesters/PluginEventLogger.h"
#include "requesters/EventLogger.h"
#include "requesters/Acquisition.h"
#include "requesters/Recipient.h"
#include <Poco/Util/Timer.h>
#include <shared/versioning/Version.h>


namespace database { namespace common {

   //--------------------------------------------------------------
   /// \Brief		   Database provider
   //--------------------------------------------------------------
   class CDataProvider : public IDataProvider
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Constructor
      /// \param [in]	databaseRequester: The database engine
      //--------------------------------------------------------------
      explicit CDataProvider(boost::shared_ptr<database::IDatabaseRequester> databaseRequester);
      
      //--------------------------------------------------------------
      /// \Brief		   Destructor
      //--------------------------------------------------------------
      virtual ~CDataProvider();

      // IDatabaseProvider implementation
      bool load();
      void unload();
      void stopMaintenanceTasks();
      boost::shared_ptr<ITransactionalProvider> getTransactionalEngine();
      boost::shared_ptr<IPluginRequester> getPluginRequester() { return m_pluginRequester; }
      boost::shared_ptr<IConfigurationRequester> getConfigurationRequester() { return m_configurationRequester; }
      boost::shared_ptr<IDeviceRequester> getDeviceRequester() { return m_deviceRequester; }
      boost::shared_ptr<IKeywordRequester> getKeywordRequester() { return m_keywordRequester; }
      boost::shared_ptr<IPageRequester> getPageRequester() { return m_pageRequester; }
      boost::shared_ptr<IWidgetRequester> getWidgetRequester() { return m_widgetRequester; }
      boost::shared_ptr<IPluginEventLoggerRequester> getPluginEventLoggerRequester() { return m_pluginEventLoggerRequester; }
      boost::shared_ptr<IEventLoggerRequester> getEventLoggerRequester() { return m_eventLoggerRequester; }
      boost::shared_ptr<IAcquisitionRequester> getAcquisitionRequester() { return m_acquisitionRequester; }
      boost::shared_ptr<IRuleRequester> getRuleRequester() { return m_ruleRequester; }
      boost::shared_ptr<IRecipientRequester> getRecipientRequester() { return m_recipientRequester; }
      // [END] IDatabaseProvider implementation

      shared::versioning::CVersion GetVersion();
   private:
      //--------------------------------------------------------------
      /// \Brief		Load all requesters
      //--------------------------------------------------------------
      void loadRequesters();
      
      //--------------------------------------------------------------
      /// \Brief		Configure the SQLite3 engine with some extended functions
      //--------------------------------------------------------------
      void registerExtendedFunctions();

      //--------------------------------------------------------------
      /// \Brief		Initialize the maintenance tasks
      //--------------------------------------------------------------
      void initializeMaintenanceTasks();

      //--------------------------------------------------------------
      /// \Brief		Initialize the summary computing task
      //--------------------------------------------------------------
      void initializeSummaryComputingTask();

      //--------------------------------------------------------------
      /// \Brief		Initialize the purge task
      //--------------------------------------------------------------
      void initializePurgeTask();

   private:
      //--------------------------------------------------------------
      /// \Brief		Plugin requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::common::requesters::CPlugin>		m_pluginRequester;
      
      //--------------------------------------------------------------
      /// \Brief		Plugin requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CConfiguration>		m_configurationRequester;

      //--------------------------------------------------------------
      /// \Brief		Device requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CDevice>		m_deviceRequester;

      //--------------------------------------------------------------
      /// \Brief		Keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CKeyword>		m_keywordRequester;

      //--------------------------------------------------------------
      /// \Brief		Page requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CPage>		m_pageRequester;

      //--------------------------------------------------------------
      /// \Brief		Widget requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CWidget>		m_widgetRequester;

      //--------------------------------------------------------------
      /// \Brief		PluginEventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CPluginEventLogger> m_pluginEventLoggerRequester;
      
      //--------------------------------------------------------------
      /// \Brief		EventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CEventLogger> m_eventLoggerRequester;

      //--------------------------------------------------------------
      /// \Brief		Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CAcquisition> m_acquisitionRequester;
      
      //--------------------------------------------------------------
      /// \Brief		Rule requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CRule> m_ruleRequester;
      
      //--------------------------------------------------------------
      /// \Brief		Recipient requester
      //--------------------------------------------------------------
      boost::shared_ptr<common::requesters::CRecipient> m_recipientRequester;

      //--------------------------------------------------------------
      /// \Brief		database requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDatabaseRequester> m_databaseRequester;

      //--------------------------------------------------------------
      /// \Brief		The maintenance timer
      //--------------------------------------------------------------
      Poco::Util::Timer m_maintenanceTimer;

      //--------------------------------------------------------------
      /// \Brief		The maintenance summary computing task
      //--------------------------------------------------------------
      Poco::Util::TimerTask::Ptr m_maintenanceSummaryComputingTask;  
      
      //--------------------------------------------------------------
      /// \Brief		The maintenance purge task
      //--------------------------------------------------------------
      Poco::Util::TimerTask::Ptr m_maintenancePurgeTask;
   };
   
} //namespace common 
} //namespace database 

