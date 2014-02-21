#pragma once

#include "server/database/IDataProvider.h"
#include "requesters/SQLiteHardwareRequester.h"
#include "requesters/SQLiteConfigurationRequester.h"
#include "requesters/SQLiteAcquisitionRequester.h"
#include "requesters/SQLiteDeviceRequester.h"
#include "requesters/SQLiteKeywordRequester.h"
#include "requesters/SQLitePageRequester.h"
#include "requesters/SQLiteWidgetRequester.h"
#include "requesters/SQLiteHardwareEventLoggerRequester.h"
#include "requesters/SQLiteEventLoggerRequester.h"
#include "SQLiteRequester.h"
#include "sqlite3.h"

namespace server { 
namespace database { 
namespace sqlite { 

   //--------------------------------------------------------------
   /// \Brief		   Database provider for SQLite
   //--------------------------------------------------------------
   class CSQLiteDataProvider : public server::database::IDataProvider
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
      boost::shared_ptr<server::database::ITransactionalProvider> getTransactionalEngine();
      boost::shared_ptr<server::database::IHardwareRequester> getHardwareRequester() { return m_hardwareRequester; }
      boost::shared_ptr<server::database::IConfigurationRequester> getConfigurationRequester() { return m_configurationRequester; }
      boost::shared_ptr<server::database::IDeviceRequester> getDeviceRequester() { return m_deviceRequester; }
      boost::shared_ptr<server::database::IAcquisitionRequester> getAcquisitionRequester() { return m_acquisitionRequester; }
      boost::shared_ptr<server::database::IKeywordRequester> getKeywordRequester() { return m_keywordRequester; }
      boost::shared_ptr<server::database::IPageRequester> getPageRequester() { return m_pageRequester; }
      boost::shared_ptr<server::database::IWidgetRequester> getWidgetRequester() { return m_widgetRequester; }
      boost::shared_ptr<server::database::IHardwareEventLoggerRequester> getHardwareEventLoggerRequester() { return m_hardwareEventLoggerRequester; }
      boost::shared_ptr<server::database::IEventLoggerRequester> getEventLoggerRequester() { return m_eventLoggerRequester; }


      // [END] IDatabaseProvider implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Load all requesters
      //--------------------------------------------------------------
      void loadRequesters();
      
   private:
      //--------------------------------------------------------------
      /// \Brief		Hardware requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteHardwareRequester>		m_hardwareRequester;
      
      //--------------------------------------------------------------
      /// \Brief		Hardware requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteConfigurationRequester>		m_configurationRequester;

      //--------------------------------------------------------------
      /// \Brief		Device requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteDeviceRequester>		m_deviceRequester;

      //--------------------------------------------------------------
      /// \Brief		Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteAcquisitionRequester>		m_acquisitionRequester;

      //--------------------------------------------------------------
      /// \Brief		Keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteKeywordRequester>		m_keywordRequester;

      //--------------------------------------------------------------
      /// \Brief		Page requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLitePageRequester>		m_pageRequester;

      //--------------------------------------------------------------
      /// \Brief		Widget requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteWidgetRequester>		m_widgetRequester;

      //--------------------------------------------------------------
      /// \Brief		HardwareEventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteHardwareEventLoggerRequester> m_hardwareEventLoggerRequester;
      
      //--------------------------------------------------------------
      /// \Brief		EventLogger requester
      //--------------------------------------------------------------
      boost::shared_ptr<server::database::sqlite::requesters::CSQLiteEventLoggerRequester> m_eventLoggerRequester;
      
      
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
} //namespace server
