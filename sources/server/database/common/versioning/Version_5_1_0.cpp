#include "stdafx.h"
#include "Version_5_1_0.h"
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
         const shared::versioning::CSemVer CVersion_5_1_0::Version(5, 1, 0);

         void CVersion_5_1_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                              const shared::versioning::CSemVer& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_5_0_0::checkForUpgrade(requester, currentVersion);

               //do update stuff
               updateFrom5_0_0(requester);
            }
            else
            {
               //good version
            }
         }

         void CVersion_5_1_0::updateFrom5_0_0(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (5.0.0 -> 5.1.0)...";

               // The aims of this update is to add HistoryDepth column to keyword table

               // Create transaction if supported
               if (requester->transactionSupport())
                  requester->transactionBegin();

               // Add column
               requester->addTableColumn(CKeywordTable::getTableName(), "historyDepth TEXT NOT NULL default \"Default\"");

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
