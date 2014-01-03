#include "stdafx.h"
#include "SQLiteVersion1.h"
#include "database/sqlite/Query.h"
#include "SQLiteSystemTables.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "tools/Version.h"



CSQLiteVersion1::CSQLiteVersion1()
{
}

CSQLiteVersion1::~CSQLiteVersion1()
{
}

// ISQLiteVersionUpgrade implementation
void CSQLiteVersion1::checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester)
{
   //check that table Configuration exists
   CQuery sCheckForConfigurationTableExists;

   sCheckForConfigurationTableExists.  SelectCount().
                                       From(CSqliteMasterTable::getTableName()).
                                       Where(CSqliteMasterTable::getTypeColumnName(), CQUERY_OP_EQUAL, SQLITEMASTER_TABLE).
                                       And(CSqliteMasterTable::getNameColumnName(), CQUERY_OP_EQUAL, CConfigurationTable::getTableName());
   int count = pRequester->queryCount(sCheckForConfigurationTableExists);

   if(count != 1)
   {
      //configuration table is missing,
      //create the full database structure
      YADOMS_LOG(info) << "-> need to create database";
      CreateDatabase(pRequester);
   }
   else
   {
      bool bNeedToUpgrade = false;
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
            bNeedToUpgrade = false;
         }
         else
         {
            //version is lower to 1.0.0.0, then upgrade database
            bNeedToUpgrade = true;
         }
      }
      else
      {
         //la version n'a jamais été configurée
         YADOMS_LOG(info) << "version has never been configured";
         bNeedToUpgrade = true;
      }

      if(bNeedToUpgrade)
      {
            YADOMS_LOG(info) << "-> need to upgrade database";
            UpgradeDatabase(pRequester);
      }
   }
}
// [END] ISQLiteVersionUpgrade implementation


//-----------------------------------
/// \brief     Create the database (when tables are not found)
///\param [in] pRequester : database requester object
//-----------------------------------
void CSQLiteVersion1::CreateDatabase(const boost::shared_ptr<CSQLiteRequester> & pRequester)
{
}

//-----------------------------------
/// \brief     Upgrade the database (missing or bad database version)
///\param [in] pRequester : database requester object
//-----------------------------------
void CSQLiteVersion1::UpgradeDatabase(const boost::shared_ptr<CSQLiteRequester> & pRequester)
{
}