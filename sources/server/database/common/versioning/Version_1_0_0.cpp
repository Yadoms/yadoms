#include "stdafx.h"
#include "Version_1_0_0.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/Version.h>
#include "VersionException.h"
#include <shared/Log.h>

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CVersion CVersion_1_0_0::Version(1, 0, 0);

         CVersion_1_0_0::CVersion_1_0_0()
         {
         }

         CVersion_1_0_0::~CVersion_1_0_0()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_1_0_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            auto bNeedToCreateOrUpgrade = false;

            if (currentVersion >= Version)
            {
               //not for me, version is correct

               //check that all tables exist
               if (!pRequester->checkTableExists(CConfigurationTable::getTableName()) ||
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
            }
            else
            {
               //version is lower to requested
               bNeedToCreateOrUpgrade = true;
            }

            if (bNeedToCreateOrUpgrade)
            {
               CreateDatabase(pRequester);
            }
         }

         void CVersion_1_0_0::updateDatabaseVersion(const boost::shared_ptr<IDatabaseRequester> pRequester,
                                                    const shared::versioning::CVersion& newVersion)
         {
            auto qUpdate = pRequester->newQuery();

            qUpdate->Update(CConfigurationTable::getTableName())
                   .Set(CConfigurationTable::getValueColumnName(), newVersion.toString())
                   .Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database")
                   .And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

            pRequester->queryStatement(*qUpdate);
         }

         // [END] ISQLiteVersionUpgrade implementation


         //-----------------------------------
         /// \brief     Create the database (when tables are not found)
         ///\param [in] pRequester : database requester object
         ///\throw      CVersionException if create database failed
         //-----------------------------------
         void CVersion_1_0_0::CreateDatabase(const boost::shared_ptr<IDatabaseRequester>& pRequester)
         {
            try
            {
               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               //delete tables
               if (!pRequester->dropTableIfExists(CConfigurationTable::getTableName()))
                  throw CVersionException("Failed to delete Configuration table");
               if (!pRequester->dropTableIfExists(CDeviceTable::getTableName()))
                  throw CVersionException("Failed to delete Device table");
               if (!pRequester->dropTableIfExists(CPluginTable::getTableName()))
                  throw CVersionException("Failed to delete Plugin table");
               if (!pRequester->dropTableIfExists(CKeywordTable::getTableName()))
                  throw CVersionException("Failed to delete Keyword table");
               if (!pRequester->dropTableIfExists(CPageTable::getTableName()))
                  throw CVersionException("Failed to delete Page table");
               if (!pRequester->dropTableIfExists(CWidgetTable::getTableName()))
                  throw CVersionException("Failed to delete Widget table");
               if (!pRequester->dropTableIfExists(CPluginEventLoggerTable::getTableName()))
                  throw CVersionException("Failed to delete PluginEventLogger table");
               if (!pRequester->dropTableIfExists(CEventLoggerTable::getTableName()))
                  throw CVersionException("Failed to delete EventLogger table");
               if (!pRequester->dropTableIfExists(CAcquisitionTable::getTableName()))
                  throw CVersionException("Failed to delete Acquisition table");
               if (!pRequester->dropTableIfExists(CAcquisitionSummaryTable::getTableName()))
                  throw CVersionException("Failed to delete AcquisitionSummary table");
               if (!pRequester->dropTableIfExists(CRuleTable::getTableName()))
                  throw CVersionException("Failed to delete rule table");
               if (!pRequester->dropTableIfExists(CRecipientTable::getTableName()))
                  throw CVersionException("Failed to delete Recipient table");
               if (!pRequester->dropTableIfExists(CRecipientFieldTable::getTableName()))
                  throw CVersionException("Failed to delete RecipientFields table");

               auto scriptProvider = pRequester->getTableCreationScriptProvider();

               //create tables
               if (!pRequester->createTableIfNotExists(CConfigurationTable::getTableName(), scriptProvider->getTableConfiguration()))
                  throw CVersionException("Failed to create Configuration table");
               if (!pRequester->createTableIfNotExists(CDeviceTable::getTableName(), scriptProvider->getTableDevice()))
                  throw CVersionException("Failed to create Device table");
               if (!pRequester->createTableIfNotExists(CPluginTable::getTableName(), scriptProvider->getTablePlugin()))
                  throw CVersionException("Failed to create Plugin table");
               if (!pRequester->createTableIfNotExists(CKeywordTable::getTableName(), scriptProvider->getTableKeyword()))
                  throw CVersionException("Failed to create Keyword table");
               if (!pRequester->createTableIfNotExists(CPageTable::getTableName(), scriptProvider->getTablePage()))
                  throw CVersionException("Failed to create Page table");
               if (!pRequester->createTableIfNotExists(CWidgetTable::getTableName(), scriptProvider->getTableWidget()))
                  throw CVersionException("Failed to create Widget table");
               if (!pRequester->createTableIfNotExists(CPluginEventLoggerTable::getTableName(), scriptProvider->getTablePluginEventLogger()))
                  throw CVersionException("Failed to create PluginEventLogger table");
               if (!pRequester->createTableIfNotExists(CEventLoggerTable::getTableName(), scriptProvider->getTableEventLogger()))
                  throw CVersionException("Failed to create EventLogger table");
               if (!pRequester->createTableIfNotExists(CAcquisitionTable::getTableName(), scriptProvider->getTableAcquisition()))
                  throw CVersionException("Failed to create Acquisition table");
               if (!pRequester->createTableIfNotExists(CAcquisitionSummaryTable::getTableName(), scriptProvider->getTableAcquisitionSummary()))
                  throw CVersionException("Failed to create AcquisitionSummary table");
               if (!pRequester->createTableIfNotExists(CRuleTable::getTableName(), scriptProvider->getTableRule()))
                  throw CVersionException("Failed to create Rule table");
               if (!pRequester->createTableIfNotExists(CRecipientTable::getTableName(), scriptProvider->getTableRecipient()))
                  throw CVersionException("Failed to create Recipient table");
               if (!pRequester->createTableIfNotExists(CRecipientFieldTable::getTableName(), scriptProvider->getTableRecipientField()))
                  throw CVersionException("Failed to create RecipientFields table");

               std::vector<std::string> indexScripts;
               scriptProvider->getTableAcquisitionIndexes(indexScripts);

               //indexes
               for (std::vector<std::string>::const_iterator i = indexScripts.begin(); i != indexScripts.end(); ++i)
               {
                  pRequester->createIndex(CAcquisitionTable::getTableName(), *i);
               }

               //set the database version
               auto qInsert = pRequester->newQuery();
               qInsert->InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDescriptionColumnName()).
                      Values("Database", "Version", Version.toString(), "Database version");
               pRequester->queryStatement(*qInsert);

               //set the developer mode
               qInsert->Clear().InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDefaultValueColumnName(), CConfigurationTable::getDescriptionColumnName()).
                      Values("system", "developerMode", "false", "false", "Developer mode");
               pRequester->queryStatement(*qInsert);

               //system plugin
               qInsert->Clear().InsertInto(CPluginTable::getTableName(), CPluginTable::getDisplayNameColumnName(), CPluginTable::getTypeColumnName(), CPluginTable::getAutoStartColumnName(), CPluginTable::getCategoryColumnName()).
                      Values("System", "System", true, database::entities::EPluginCategory::kSystem);
               pRequester->queryStatement(*qInsert);

               //commit transaction
               if (pRequester->transactionSupport())
                  pRequester->transactionCommit();

               //compact database
               pRequester->vacuum();
            }
            catch (CVersionException& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to create database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


