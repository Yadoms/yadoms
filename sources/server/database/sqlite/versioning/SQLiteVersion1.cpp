#include "stdafx.h"
#include "SQLiteVersion1.h"
#include "database/sqlite/Query.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "tools/Version.h"
#include "SQLiteVersionException.h"



CSQLiteVersion1::CSQLiteVersion1()
{
}

CSQLiteVersion1::~CSQLiteVersion1()
{
}

// ISQLiteVersionUpgrade implementation
void CSQLiteVersion1::checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester)
{
   bool bNeedToCreateOrUpgrade = true;

   //get the database version
   CQuery qVersion;
   qVersion.Select(CConfigurationTable::getValueColumnName()).
      From(CConfigurationTable::getTableName()).
      Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database").
      And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

   CSingleValueAdapter<std::string> adapter;
   pRequester->queryEntities<std::string>(&adapter, qVersion);
   std::vector<std::string> results = adapter.getResults();
   if(results.size() >= 1)
   {
      //une version est déjà configurée
      CVersion versionFromdatabase(results[0]);
      YADOMS_LOG(info) << "database version  : " << results[0];
      if(versionFromdatabase >= CVersion(1,0,0,0))
      {
         //not for me, version is correct
         bNeedToCreateOrUpgrade = false;
      }
      else
      {
         //version is lower to 1.0.0.0, then create database
         bNeedToCreateOrUpgrade = true;
      }
   }
   else
   {
      //la version n'a jamais été configurée
      YADOMS_LOG(info) << "version has never been configured";
      bNeedToCreateOrUpgrade = true;
   }

   /*
   //check that table all tables exists
   if(!pRequester->checkTableExists(CAcquisitionTable::getTableName()) ||
   !pRequester->checkTableExists(CConfigurationTable::getTableName()) ||
   !pRequester->checkTableExists(CDeviceTable::getTableName()) ||
   !pRequester->checkTableExists(CHardwareTable::getTableName()) ||
   !pRequester->checkTableExists(CKeywordTable::getTableName()))
   {
   //configuration table is missing,
   //create the full database structure
   YADOMS_LOG(info) << "-> need to create database";
   bNeedToCreateOrUpgrade = true;
   }
   else
   {


   }*/

   if(bNeedToCreateOrUpgrade)
   {
      CreateDatabase(pRequester);
   }

}
// [END] ISQLiteVersionUpgrade implementation


//-----------------------------------
/// \brief     Create the database (when tables are not found)
///\param [in] pRequester : database requester object
///\throw      CSQLiteVersionException if create database failed
//-----------------------------------
void CSQLiteVersion1::CreateDatabase(const boost::shared_ptr<CSQLiteRequester> & pRequester)
{
   try
   {
      //create transaction
      pRequester->transactionBegin();

      //delete tables
      if(!pRequester->dropTableIfExists(CAcquisitionTable::getTableName()))
         throw CSQLiteVersionException("Failed to delete Acquisition table");
      if(!pRequester->dropTableIfExists(CConfigurationTable::getTableName()))
         throw CSQLiteVersionException("Failed to delete Configuration table");
      if(!pRequester->dropTableIfExists(CDeviceTable::getTableName()))
         throw CSQLiteVersionException("Failed to delete Device table");
      if(!pRequester->dropTableIfExists(CHardwareTable::getTableName()))
         throw CSQLiteVersionException("Failed to delete Hardware table");
      if(!pRequester->dropTableIfExists(CKeywordTable::getTableName()))
         throw CSQLiteVersionException("Failed to delete Keyword table");

      //create tables
      if(!pRequester->createTableIfNotExists(CAcquisitionTable::getTableName(), CAcquisitionTable::getTableCreationScript()))
         throw CSQLiteVersionException("Failed to create Acquisition table");
      if(!pRequester->createTableIfNotExists(CConfigurationTable::getTableName(), CConfigurationTable::getTableCreationScript()))
         throw CSQLiteVersionException("Failed to create Configuration table");
      if(!pRequester->createTableIfNotExists(CDeviceTable::getTableName(), CDeviceTable::getTableCreationScript()))
         throw CSQLiteVersionException("Failed to create Device table");
      if(!pRequester->createTableIfNotExists(CHardwareTable::getTableName(), CHardwareTable::getTableCreationScript()))
         throw CSQLiteVersionException("Failed to create Hardware table");
      if(!pRequester->createTableIfNotExists(CKeywordTable::getTableName(), CKeywordTable::getTableCreationScript()))
         throw CSQLiteVersionException("Failed to create Keyword table");

      //set the database version
      CQuery qInsert;
      qInsert.InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDescriptionColumnName()).
         Values( "Database", "Version", 1.0, "Database version");
      pRequester->query(qInsert);

      //commit transaction
      pRequester->transactionCommit();
   }
   catch(CSQLiteVersionException & ex)
   {
      YADOMS_LOG(fatal) << "Failed to upgrade database : " << ex.what();
      YADOMS_LOG(fatal) << "Rollback transaction";
      pRequester->transactionRollback();
      throw CSQLiteVersionException("Failed to create database");
   }

}
