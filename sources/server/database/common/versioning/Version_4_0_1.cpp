#include "stdafx.h"
#include "Version_4_0_1.h"
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
         const shared::versioning::CVersion CVersion_4_0_1::Version(4, 0, 1);

         CVersion_4_0_1::CVersion_4_0_1()
         {
         }

         CVersion_4_0_1::~CVersion_4_0_1()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_4_0_1::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_4_0_0::checkForUpgrade(pRequester, currentVersion);

               //do update stuff
               UpdateFrom4_0_0(pRequester);
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
         void CVersion_4_0_1::UpdateFrom4_0_0(const boost::shared_ptr<IDatabaseRequester>& pRequester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.0.0 -> 4.0.1)";

               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               auto qUpdateNameColumn = pRequester->newQuery();
               auto qUpdateFriendlyNameColumn = pRequester->newQuery();
               auto qUpdateCapacityNameColumn = pRequester->newQuery();

               // Change the name from rssi to signalStrength
               qUpdateNameColumn.Update(CKeywordTable::getTableName()).
                  Set(CKeywordTable::getNameColumnName(), "signalPower").
                  Where(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, "rssi");

               // Change the friendlyName  from rssi to signalStrength
               qUpdateFriendlyNameColumn.Update(CKeywordTable::getTableName()).
                  Set(CKeywordTable::getFriendlyNameColumnName(), "signalPower").
                  Where(CKeywordTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, "rssi");

               // Change the capacityName  from rssi to signalStrength
               qUpdateCapacityNameColumn.Update(CKeywordTable::getTableName()).
                  Set(CKeywordTable::getCapacityNameColumnName(), "signalPower").
                  Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, "rssi");

               pRequester->queryStatement(qUpdateNameColumn);
               pRequester->queryStatement(qUpdateFriendlyNameColumn);
               pRequester->queryStatement(qUpdateCapacityNameColumn);

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
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.0.0 -> 4.0.1) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 