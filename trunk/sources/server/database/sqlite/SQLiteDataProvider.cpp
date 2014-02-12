#include "stdafx.h"

#include "SQLiteDataProvider.h"
#include "SQLiteHardwareRequester.h"
#include "SQLiteConfigurationRequester.h"
#include "sqlite3.h"
#include <boost/filesystem.hpp>
#include "SQLiteRequester.h"
#include <shared/Log.h>
#include "versioning/SQLiteVersionUpgraderFactory.h"
#include "versioning/SQLiteVersionException.h"
#include <shared/Exceptions/NotSupportedException.hpp>

CSQLiteDataProvider::CSQLiteDataProvider(const std::string & dbFile)
   :m_dbFile(dbFile), m_pDatabaseHandler(NULL)
{
   //check that library is comiled with THREAD_SAFE= 1 (ensure that it is full mutex access)
   if(sqlite3_threadsafe() != 1)
      throw CNotSupportedException("SQLite3 non threadsafe");
}

CSQLiteDataProvider::~CSQLiteDataProvider()
{
   unload();
}


// IDatabaseProvider implementation
bool CSQLiteDataProvider::load()
{
   YADOMS_LOG(info) << "Load database";

   bool result = false;

   try
   {
      if ( !boost::filesystem::exists( m_dbFile.c_str() ) )
      {
         YADOMS_LOG(info) << "Database file is not found : " << m_dbFile;
         YADOMS_LOG(info) << "Yadoms will create a blank one";
      }

      int rc = sqlite3_open(m_dbFile.c_str(), &m_pDatabaseHandler);

      if(rc)
      {
         YADOMS_LOG(fatal) << "Fail to load database: " << sqlite3_errmsg(m_pDatabaseHandler);
         sqlite3_close(m_pDatabaseHandler);
         result = false;
      }
      else
      {
         //db loaded with succes, create the SQLiteRequester (can execute sql queries) 
         m_databaseRequester.reset(new CSQLiteRequester(m_pDatabaseHandler));

         //check for update
         YADOMS_LOG(info) << "Check for database update...";
         CSQLiteVersionUpgraderFactory::GetUpgrader()->checkForUpgrade(m_databaseRequester);

         //create entities requester (high level querier)
         loadRequesters();

         YADOMS_LOG(info) << "Load database with success";
         result = true;

      }
   }
   catch(CSQLiteVersionException & exc)
   {
      YADOMS_LOG(error) << "Fail to load database (upgrade error) : " << std::endl << exc.what();
      if(m_pDatabaseHandler != NULL)
         sqlite3_close(m_pDatabaseHandler);
      result = false;
   }
   catch(...)
   {
      YADOMS_LOG(error) << "Unknown exception while loading database";
      if(m_pDatabaseHandler != NULL)
         sqlite3_close(m_pDatabaseHandler);
      result = false;
   }
   return result;
}



void CSQLiteDataProvider::unload()
{
   if(m_pDatabaseHandler != NULL)
      sqlite3_close(m_pDatabaseHandler);  
}

// [END] IHardwarePlugin implementation


void CSQLiteDataProvider::loadRequesters()
{
   m_hardwareRequester.reset(new CSQLiteHardwareRequester(*this, m_databaseRequester));
   m_configurationRequester.reset(new CSQLiteConfigurationRequester(*this, m_databaseRequester));
   m_deviceRequester.reset(new CSQLiteDeviceRequester(*this, m_databaseRequester));
   m_acquisitionRequester.reset(new CSQLiteAcquisitionRequester(*this, m_databaseRequester));
   m_keywordRequester.reset(new CSQLiteKeywordRequester(*this, m_databaseRequester));
   m_pageRequester.reset(new CSQLitePageRequester(*this, m_databaseRequester));
   m_widgetRequester.reset(new CSQLiteWidgetRequester(*this, m_databaseRequester));
   m_hardwareEventLoggerRequester.reset(new CSQLiteHardwareEventLoggerRequester(*this, m_databaseRequester));
}



boost::shared_ptr<ITransactionalProvider> CSQLiteDataProvider::getTransactionalEngine() 
{
   if(!m_databaseRequester->transactionIsAlreadyCreated())
      return m_databaseRequester; 
}