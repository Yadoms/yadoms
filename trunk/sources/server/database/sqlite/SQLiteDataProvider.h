#pragma once

#include "server/database/IDataProvider.h"
#include "server/database/IDataBackup.h"
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
#include "SQLiteRequester.h"
#include "sqlite3.h"


namespace database { 
namespace sqlite { 

   //--------------------------------------------------------------
   /// \Brief		   Database provider for SQLite
   //--------------------------------------------------------------
   class CSQLiteDataProvider : public IDataProvider, public IDataBackup
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Constructor
      /// \param [in]	pConfig: startup configuration
      /// \throw        shared::exception::CNotSupported if the linked SQLIte library is compiled without ThreadSafe options
      //--------------------------------------------------------------
      CSQLiteDataProvider(const std::string & dbFile);
      
      //--------------------------------------------------------------
      /// \Brief		   Destructor
      //--------------------------------------------------------------
      virtual ~CSQLiteDataProvider();

      // IDatabaseProvider implementation
      bool load();
      void unload();
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

      // IDataBackup implementation
      void backupData(const std::string & backupLocation, ProgressFunc reporter);
      // [END] IDataBackup implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Load all requesters
      //--------------------------------------------------------------
      void loadRequesters();
      
      //--------------------------------------------------------------
      /// \Brief		Configure the SQLite3 engine with some extended functions
      //--------------------------------------------------------------
      void registerExtendedFunctions();

   private:
      //--------------------------------------------------------------
      /// \Brief		Plugin requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CPlugin>		m_pluginRequester;
      
      //--------------------------------------------------------------
      /// \Brief		Plugin requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CConfiguration>		m_configurationRequester;

      //--------------------------------------------------------------
      /// \Brief		Device requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CDevice>		m_deviceRequester;

      //--------------------------------------------------------------
      /// \Brief		Keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CKeyword>		m_keywordRequester;

      //--------------------------------------------------------------
      /// \Brief		Page requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CPage>		m_pageRequester;

      //--------------------------------------------------------------
      /// \Brief		Widget requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CWidget>		m_widgetRequester;

      //--------------------------------------------------------------
      /// \Brief		PluginEventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CPluginEventLogger> m_pluginEventLoggerRequester;
      
      //--------------------------------------------------------------
      /// \Brief		EventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CEventLogger> m_eventLoggerRequester;

      //--------------------------------------------------------------
      /// \Brief		Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CAcquisition> m_acquisitionRequester;   
      
      //--------------------------------------------------------------
      /// \Brief		Rule requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CRule> m_ruleRequester;       
      
      //--------------------------------------------------------------
      /// \Brief		Recipient requester
      //--------------------------------------------------------------
      boost::shared_ptr<requesters::CRecipient> m_recipientRequester;   
      
      
      //--------------------------------------------------------------
      /// \Brief		Startup options
      //--------------------------------------------------------------
      const std::string m_dbFile;

      //--------------------------------------------------------------
      /// \Brief		database handler
      //--------------------------------------------------------------
      sqlite3 * m_pDatabaseHandler; //not use shared_ptr because not alllocated by this library

      //--------------------------------------------------------------
      /// \Brief		database requester (for SQL queries)
      //--------------------------------------------------------------
      boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
   };
   

} //namespace sqlite
} //namespace database 

