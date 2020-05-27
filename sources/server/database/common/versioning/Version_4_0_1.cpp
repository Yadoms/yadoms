#include "stdafx.h"
#include "Version_4_0_1.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/SemVer.h>
#include "VersionException.h"
#include <shared/Log.h>

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CSemVer CVersion_4_0_1::Version(4, 0, 1);

         CVersion_4_0_1::CVersion_4_0_1()
         {
         }

         CVersion_4_0_1::~CVersion_4_0_1()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_4_0_1::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                              const shared::versioning::CSemVer& currentVersion)
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
         void CVersion_4_0_1::UpdateFrom4_0_0(const boost::shared_ptr<IDatabaseRequester>& pRequester) const
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.0.0 -> 4.0.1)...";

               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               auto qUpdateKeyword = pRequester->newQuery();

               // Change the name from rssi to signalStrength
               qUpdateKeyword->Update(CKeywordTable::getTableName()).
                  Set(CKeywordTable::getNameColumnName(), "signalPower",
                      CKeywordTable::getFriendlyNameColumnName(), "signalPower",
                      CKeywordTable::getCapacityNameColumnName(), "signalPower").
                  Where(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, "rssi");

               pRequester->queryStatement(*qUpdateKeyword);

               //set the database version
               updateDatabaseVersion(pRequester, Version);

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


