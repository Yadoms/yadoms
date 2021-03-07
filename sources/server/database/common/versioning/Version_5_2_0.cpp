#include "stdafx.h"
#include "Version_5_2_0.h"
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
         const shared::versioning::CSemVer CVersion_5_2_0::Version(5, 2, 0);

         void CVersion_5_2_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                              const shared::versioning::CSemVer& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_5_1_0::checkForUpgrade(requester, currentVersion);

               //do update stuff
               updateFrom5_1_0(requester);
            }
            else
            {
               //good version
            }
         }

         void CVersion_5_2_0::updateFrom5_1_0(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (5.1.0 -> 5.2.0)...";

               // The aims of this update is to rename some capacities :
               // - pluginState_capacity to pluginState 
               // - pluginStateMessage_capacity to pluginStateMessage
               // - deviceState_capacity to deviceState 
               // - deviceStateMessage_capacity to deviceStateMessage

               // Create transaction if supported
               if (requester->transactionSupport())
                  requester->transactionBegin();

               // update Keyword set capacityName == "pluginState" where capacityName == "pluginState_capacity"
               auto q1 = requester->newQuery();
               q1->Update(CKeywordTable::getTableName())
                .Set(CKeywordTable::getCapacityNameColumnName(), "pluginState")
                .Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, "pluginState_capacity");
               requester->queryStatement(*q1, false);

               // update Keyword set capacityName == "pluginStateMessage" where capacityName == "pluginStateMessage_capacity"
               auto q2 = requester->newQuery();
               q2->Update(CKeywordTable::getTableName())
                  .Set(CKeywordTable::getCapacityNameColumnName(), "pluginStateMessage")
                  .Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, "pluginStateMessage_capacity");
               requester->queryStatement(*q2, false);

               // update Keyword set capacityName == "deviceState" where capacityName == "deviceState_capacity"
               auto q3 = requester->newQuery();
               q3->Update(CKeywordTable::getTableName())
                  .Set(CKeywordTable::getCapacityNameColumnName(), "deviceState")
                  .Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, "deviceState_capacity");
               requester->queryStatement(*q3, false);

               // update Keyword set capacityName == "deviceStateMessage" where capacityName == "deviceStateMessage_capacity"
               auto q4 = requester->newQuery();
               q4->Update(CKeywordTable::getTableName())
                  .Set(CKeywordTable::getCapacityNameColumnName(), "deviceStateMessage")
                  .Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, "deviceStateMessage_capacity");
               requester->queryStatement(*q4, false);

               updateDatabaseVersion(requester, Version);

               // Commit transaction
               if (requester->transactionSupport())
                  requester->transactionCommit();
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (5.0.0 -> 5.1.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (requester->transactionSupport())
                  requester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 
