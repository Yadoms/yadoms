#include "stdafx.h"
#include "SQLiteVersion1.h"
#include "database/sqlite/Query.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include <shared/versioning/Version.h>
#include "SQLiteVersionException.h"


   namespace database { 
      namespace sqlite { 
         namespace versioning { 


            CSQLiteVersion1::CSQLiteVersion1()
               :m_version(0, 2, 0, 0) //modify this version to a greater value, to force update of current version
            {
            }

            CSQLiteVersion1::~CSQLiteVersion1()
            {
            }

            // ISQLiteVersionUpgrade implementation
            void CSQLiteVersion1::checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester, const shared::versioning::CVersion & currentVersion)
            {
               bool bNeedToCreateOrUpgrade = true;

               if (currentVersion >= m_version)
               {
                  //not for me, version is correct

                  //check that table all tables exists
                  if(!pRequester->checkTableExists(CConfigurationTable::getTableName()) ||
                     !pRequester->checkTableExists(CDeviceTable::getTableName()) ||
                     !pRequester->checkTableExists(CPluginTable::getTableName()) ||
                     !pRequester->checkTableExists(CKeywordTable::getTableName()) ||
                     !pRequester->checkTableExists(CPageTable::getTableName()) ||
                     !pRequester->checkTableExists(CWidgetTable::getTableName()) ||
                     !pRequester->checkTableExists(CPluginEventLoggerTable::getTableName()) ||
                     !pRequester->checkTableExists(CEventLoggerTable::getTableName()) ||
                     !pRequester->checkTableExists(CAcquisitionTable::getTableName()) ||
                     !pRequester->checkTableExists(CAcquisitionSummaryTable::getTableName()) ||
                     !pRequester->checkTableExists(CRuleTable::getTableName()) || 
                     !pRequester->checkTableExists(CRecipientTable::getTableName()) || 
                     !pRequester->checkTableExists(CRecipientFieldTable::getTableName()))
                  {
                     //at least one table is missing
                     bNeedToCreateOrUpgrade = true;
                  }
                  else
                  {
                     //good version, but missing table (developpement mode only)
                     bNeedToCreateOrUpgrade = false;
                  }
               }
               else
               {
                  //version is lower to requested, then create database
                  bNeedToCreateOrUpgrade = true;
               }

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
                  if(!pRequester->dropTableIfExists(CConfigurationTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Configuration table");
                  if(!pRequester->dropTableIfExists(CDeviceTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Device table");
                  if(!pRequester->dropTableIfExists(CPluginTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Plugin table");
                  if(!pRequester->dropTableIfExists(CKeywordTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Keyword table");
                  if(!pRequester->dropTableIfExists(CPageTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Page table");
                  if(!pRequester->dropTableIfExists(CWidgetTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Widget table");
                  if(!pRequester->dropTableIfExists(CPluginEventLoggerTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete PluginEventLogger table");
                  if(!pRequester->dropTableIfExists(CEventLoggerTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete EventLogger table");
                  if(!pRequester->dropTableIfExists(CAcquisitionTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Acquisition table");
                  if(!pRequester->dropTableIfExists(CAcquisitionSummaryTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete AcquisitionSummary table");
                  if (!pRequester->dropTableIfExists(CRuleTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete rule table");
                  if (!pRequester->dropTableIfExists(CRecipientTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete Recipient table");
                  if (!pRequester->dropTableIfExists(CRecipientFieldTable::getTableName()))
                     throw CSQLiteVersionException("Failed to delete RecipientFields table");

                  //create tables
                  if(!pRequester->createTableIfNotExists(CConfigurationTable::getTableName(), CConfigurationTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Configuration table");
                  if(!pRequester->createTableIfNotExists(CDeviceTable::getTableName(), CDeviceTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Device table");
                  if(!pRequester->createTableIfNotExists(CPluginTable::getTableName(), CPluginTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Plugin table");
                  if(!pRequester->createTableIfNotExists(CKeywordTable::getTableName(), CKeywordTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Keyword table");
                  if(!pRequester->createTableIfNotExists(CPageTable::getTableName(), CPageTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Page table");
                  if(!pRequester->createTableIfNotExists(CWidgetTable::getTableName(), CWidgetTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Widget table");
                  if(!pRequester->createTableIfNotExists(CPluginEventLoggerTable::getTableName(), CPluginEventLoggerTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create PluginEventLogger table");
                  if(!pRequester->createTableIfNotExists(CEventLoggerTable::getTableName(), CEventLoggerTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create EventLogger table");
                  if(!pRequester->createTableIfNotExists(CAcquisitionTable::getTableName(), CAcquisitionTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Acquisition table");
                  if (!pRequester->createTableIfNotExists(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create AcquisitionSummary table");
                  if (!pRequester->createTableIfNotExists(CRuleTable::getTableName(), CRuleTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Rule table");
						if (!pRequester->createTableIfNotExists(CRecipientTable::getTableName(), CRecipientTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create Recipient table");
						if (!pRequester->createTableIfNotExists(CRecipientFieldTable::getTableName(), CRecipientFieldTable::getTableCreationScript()))
                     throw CSQLiteVersionException("Failed to create RecipientFields table");

                  //set the database version
                  CQuery qInsert;
                  qInsert.InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDescriptionColumnName()).
                     Values("Database", "Version", m_version.toString(), "Database version");
                  pRequester->queryStatement(qInsert);              
                  
                  //set the developer mode
                  qInsert.Clear().InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDefaultValueColumnName(), CConfigurationTable::getDescriptionColumnName()).
                     Values( "system", "developerMode", "false", "false", "Developer mode");
                  pRequester->queryStatement(qInsert);

                  //system plugin
                  qInsert.Clear().InsertInto(CPluginTable::getTableName(), CPluginTable::getDisplayNameColumnName(), CPluginTable::getTypeColumnName(), CPluginTable::getAutoStartColumnName(), CPluginTable::getCategoryColumnName()).
                     Values("System", "System", true, database::entities::EPluginCategory::kSystem);
                  pRequester->queryStatement(qInsert);

                  //commit transaction
                  pRequester->transactionCommit();

                  //compact database
                  pRequester->vacuum();
               }
               catch(CSQLiteVersionException & ex)
               {
                  YADOMS_LOG(fatal) << "Failed to upgrade database : " << ex.what();
                  YADOMS_LOG(fatal) << "Rollback transaction";
                  pRequester->transactionRollback();
                  throw CSQLiteVersionException("Failed to create database");
               }
            }

         } //namespace versioning
      } //namespace sqlite
   } //namespace database 

