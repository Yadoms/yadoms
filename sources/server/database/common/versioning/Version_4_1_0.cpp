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
               YADOMS_LOG(information) << "Upgrading database (4.0.1 -> 4.1.0)";

               // List all keywords from acquisition table for later use
               auto getKeywordListQuery = pRequester->newQuery();
               getKeywordListQuery->Select(getKeywordListQuery->distinct(CAcquisitionTable::getKeywordIdColumnName()))
                  .From(CAcquisitionTable::getTableName());
               std::vector<int> keywordList;
               adapters::CSingleValueAdapterWithContainer<int> intVectorAdapter(keywordList);
               pRequester->queryEntities(&intVectorAdapter, *getKeywordListQuery);

               // Create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               // Create the 2 columns of last acquisition in keyword table
               pRequester->addTableColumn(CKeywordTable::getTableName(), "lastAcquisitionValue TEXT");
               pRequester->addTableColumn(CKeywordTable::getTableName(), "lastAcquisitionDate TEXT");

               // Populate these columns with last acquisition date/value from acquisition table.
               // Do it for each keyword ID.
               for (const auto& keywordId : keywordList)
               {
                  auto qSelect = pRequester->newQuery();
                  qSelect->Select().
                     From(CAcquisitionTable::getTableName()).
                     Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                     OrderBy(CAcquisitionTable::getDateColumnName(), CQuery::kDesc).
                     Limit(1);

                  adapters::CAcquisitionAdapter adapter;
                  pRequester->queryEntities(&adapter, *qSelect);

                  if (adapter.getResults().size() >= 1)
                  {
                     auto lastAcquisition = adapter.getResults()[0];

                     auto qUpdate = pRequester->newQuery();
                     qUpdate->Update(CKeywordTable::getTableName())
                        .Set(CKeywordTable::getLastAcquisitionDateColumnName(), lastAcquisition->Date(),
                           CKeywordTable::getLastAcquisitionValueColumnName(), lastAcquisition->Value())
                        .Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

                     pRequester->queryStatement(*qUpdate);
                  }
               }

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
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.0.1 -> 4.0.2) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


