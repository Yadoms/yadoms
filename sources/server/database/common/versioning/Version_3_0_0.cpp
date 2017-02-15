#include "stdafx.h"
#include "Version_3_0_0.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/Version.h>
#include "VersionException.h"
#include <shared/Log.h>
#include "database/sqlite/SQLiteRequester.h"

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CVersion CVersion_3_0_0::Version(3, 0, 0);

         CVersion_3_0_0::CVersion_3_0_0()
         {
         }

         CVersion_3_0_0::~CVersion_3_0_0()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_3_0_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester, const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_2_0_0::checkForUpgrade(pRequester, currentVersion);

               //do update stuff
               UpdateFrom2_0_0(pRequester);
            }
            else
            {
               //good version
            }
         }

         // [END] ISQLiteVersionUpgrade implementation


         //-----------------------------------
         /// \brief     Update from previous version
         ///\param [in] pRequester : database requester object
         ///\throw      CVersionException if create database failed
         //-----------------------------------
         void CVersion_3_0_0::UpdateFrom2_0_0(const boost::shared_ptr<IDatabaseRequester>& pRequester) const
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (2.0.0 -> 3.0.0)";

               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();


               //column
               std::string tableScript;
               pRequester->dropTableIfExists(CPluginEventLoggerTable::getTableName());
               if (boost::dynamic_pointer_cast<sqlite::CSQLiteRequester>(pRequester) != nullptr)
                  tableScript = "CREATE TABLE PluginEventLogger                        \
                           (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                              eventDate  TEXT NOT NULL,                          \
                              pluginName TEXT NOT NULL,                          \
                              pluginVersion TEXT NOT NULL,                       \
                              eventType  TEXT DEFAULT \"unload\",                \
                              message  TEXT                                      \
                           )";
               else
                  tableScript = "CREATE TABLE PluginEventLogger                        \
                           (  id SERIAL NOT NULL PRIMARY KEY,                    \
                              eventDate  TEXT NOT NULL,                          \
                              pluginName TEXT NOT NULL,                          \
                              pluginVersion TEXT NOT NULL,                       \
                              eventType  TEXT DEFAULT 'unload',                  \
                              message  TEXT                                      \
                           )";

               if (!pRequester->createTableIfNotExists(CPluginEventLoggerTable::getTableName(), tableScript))
                  throw CVersionException("Fail to create table PluginEventLogger");

               //set the database version
               updateDatabaseVersion(pRequester,
                                     Version);

               //commit transaction
               if (pRequester->transactionSupport())
                  pRequester->transactionCommit();

               //compact database
               pRequester->vacuum();
            }
            catch (CVersionException& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (2.0.0 -> 3.0.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


