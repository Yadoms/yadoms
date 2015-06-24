#pragma once

#include "server/database/IDataProvider.h"
#include "server/database/IDataBackup.h"
#include "requesters/Plugin.h"
#include "requesters/Configuration.h"
#include "requesters/Device.h"
#include "requesters/Keyword.h"
#include "requesters/Page.h"
#include "requesters/Widget.h"
#include "requesters/PluginEventLogger.h"
#include "requesters/EventLogger.h"
#include "requesters/Acquisition.h"
#include "SQLiteRequester.h"
#include "sqlite3.h"


namespace database { 
namespace sqlite { 

   //--------------------------------------------------------------
   /// \Brief		   Database provider for SQLite
   //--------------------------------------------------------------
   class CSQLiteDataProvider : public database::IDataProvider, public database::IDataBackup
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
      boost::shared_ptr<database::ITransactionalProvider> getTransactionalEngine();
      boost::shared_ptr<database::IPluginRequester> getPluginRequester() { return m_pluginRequester; }
      boost::shared_ptr<database::IConfigurationRequester> getConfigurationRequester() { return m_configurationRequester; }
      boost::shared_ptr<database::IDeviceRequester> getDeviceRequester() { return m_deviceRequester; }
      boost::shared_ptr<database::IKeywordRequester> getKeywordRequester() { return m_keywordRequester; }
      boost::shared_ptr<database::IPageRequester> getPageRequester() { return m_pageRequester; }
      boost::shared_ptr<database::IWidgetRequester> getWidgetRequester() { return m_widgetRequester; }
      boost::shared_ptr<database::IPluginEventLoggerRequester> getPluginEventLoggerRequester() { return m_pluginEventLoggerRequester; }
      boost::shared_ptr<database::IEventLoggerRequester> getEventLoggerRequester() { return m_eventLoggerRequester; }
      boost::shared_ptr<database::IAcquisitionRequester> getAcquisitionRequester() { return m_acquisitionRequester; }
      // [END] IDatabaseProvider implementation

      // IDataBackup implementation
      void backupData(const std::string & backupLocation, ProgressFunc reporter);
      // [END] IDataBackup implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Load all requesters
      //--------------------------------------------------------------
      void loadRequesters();
      
   private:
      //--------------------------------------------------------------
      /// \Brief		Plugin requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CPlugin>		m_pluginRequester;
      
      //--------------------------------------------------------------
      /// \Brief		Plugin requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CConfiguration>		m_configurationRequester;

      //--------------------------------------------------------------
      /// \Brief		Device requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CDevice>		m_deviceRequester;

      //--------------------------------------------------------------
      /// \Brief		Keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CKeyword>		m_keywordRequester;

      //--------------------------------------------------------------
      /// \Brief		Page requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CPage>		m_pageRequester;

      //--------------------------------------------------------------
      /// \Brief		Widget requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CWidget>		m_widgetRequester;

      //--------------------------------------------------------------
      /// \Brief		PluginEventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CPluginEventLogger> m_pluginEventLoggerRequester;
      
      //--------------------------------------------------------------
      /// \Brief		EventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CEventLogger> m_eventLoggerRequester;

      //--------------------------------------------------------------
      /// \Brief		Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::sqlite::requesters::CAcquisition> m_acquisitionRequester;
      
      
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

