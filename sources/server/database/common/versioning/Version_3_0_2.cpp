#include "stdafx.h"
#include "Version_3_0_2.h"
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
         const shared::versioning::CVersion CVersion_3_0_2::Version(3, 0, 2);

         CVersion_3_0_2::CVersion_3_0_2()
         {
         }

         CVersion_3_0_2::~CVersion_3_0_2()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_3_0_2::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_3_0_1::checkForUpgrade(pRequester, currentVersion);

               //do update stuff
               UpdateFrom3_0_1(pRequester);
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
         void CVersion_3_0_2::UpdateFrom3_0_1(const boost::shared_ptr<IDatabaseRequester>& pRequester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (3.0.1 -> 3.0.2)";

               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               auto qUpdate1 = pRequester->newQuery();
               auto qUpdate2 = pRequester->newQuery();

               // Change the name from rssi to signalStrength
               qUpdate1.Update(CKeywordTable::getTableName()).
                  Set(CKeywordTable::getNameColumnName(), "signalStrength").
                  Where(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, "rssi");

               // Change the friendlyName  from rssi to signalStrength
               qUpdate2.Update(CKeywordTable::getTableName()).
                  Set(CKeywordTable::getFriendlyNameColumnName(), "signalStrength").
                  Where(CKeywordTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, "rssi");

               pRequester->queryStatement(qUpdate1);
               pRequester->queryStatement(qUpdate2);

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
               YADOMS_LOG(fatal) << "Failed to upgrade database (3.0.1 -> 3.0.2) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 