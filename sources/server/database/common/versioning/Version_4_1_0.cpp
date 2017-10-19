#include "stdafx.h"
#include "Version_4_1_0.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/Version.h>
#include "VersionException.h"
#include <shared/Log.h>
#include <database/common/adapters/SingleValueAdapter.hpp>
#include <database/common/adapters/DatabaseAdapters.h>

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CVersion CVersion_4_1_0::Version(4, 1, 0);

         CVersion_4_1_0::CVersion_4_1_0()
         {
         }

         CVersion_4_1_0::~CVersion_4_1_0()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_4_1_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_4_0_1::checkForUpgrade(pRequester, currentVersion);

               //do update stuff
               UpdateFrom4_0_1(pRequester);
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
         void CVersion_4_1_0::UpdateFrom4_0_1(const boost::shared_ptr<IDatabaseRequester>& pRequester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.0.1 -> 4.1.0)...";

               // Create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               // Create the 2 columns of last acquisition in keyword table
               pRequester->addTableColumn(CKeywordTable::getTableName(), "lastAcquisitionValue TEXT");
               pRequester->addTableColumn(CKeywordTable::getTableName(), "lastAcquisitionDate TEXT");

               // Populate these columns with last acquisition date/value from acquisition table.
               // Do it for each keyword ID.
               // Request is like :
               // UPDATE Keyword
               // SET lastAcquisitionDate = (SELECT date FROM Acquisition WHERE keywordId = id ORDER BY date DESC LIMIT 1),
               //     lastAcquisitionValue = (SELECT value FROM Acquisition WHERE keywordId = id ORDER BY date DESC LIMIT 1)
               auto sub1 = pRequester->newQuery();
               sub1->Select(CAcquisitionTable::getDateColumnName()).
                     From(CAcquisitionTable::getTableName()).
                     Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, CKeywordTable::getIdColumnName()).
                     OrderBy(CAcquisitionTable::getDateColumnName(), CQuery::E_OrderWay::kDesc).
                     Limit(1);

               auto sub2 = pRequester->newQuery();
               sub2->Select(CAcquisitionTable::getValueColumnName()).
                     From(CAcquisitionTable::getTableName()).
                     Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, CKeywordTable::getIdColumnName()).
                     OrderBy(CAcquisitionTable::getDateColumnName(), CQuery::E_OrderWay::kDesc).
                     Limit(1);

               auto qUpdate = pRequester->newQuery();
               qUpdate->Update(CKeywordTable::getTableName()).
                        Set(CKeywordTable::getLastAcquisitionDateColumnName(), *sub1, 
                            CKeywordTable::getLastAcquisitionValueColumnName(), *sub2);

               pRequester->queryStatement(*qUpdate);

               // Set the database version
               updateDatabaseVersion(pRequester, Version);

               // Commit transaction
               if (pRequester->transactionSupport())
                  pRequester->transactionCommit();

               // Compact database
               pRequester->vacuum();
            }
            catch (CVersionException& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.0.1 -> 4.1.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


