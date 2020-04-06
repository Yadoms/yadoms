#include "stdafx.h"
#include "Version_5_0_0.h"
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
         const shared::versioning::CSemVer CVersion_5_0_0::Version(5, 0, 0);

         CVersion_5_0_0::CVersion_5_0_0()
         {
         }

         CVersion_5_0_0::~CVersion_5_0_0()
         {
         }

         void CVersion_5_0_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                              const shared::versioning::CSemVer& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_4_4_0::checkForUpgrade(requester, currentVersion);

               //do update stuff
               updateFrom4_4_0(requester);
            }
            else
            {
               //good version
            }
         }

         void CVersion_5_0_0::updateFrom4_4_0(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.4.0 -> 5.0.0)...";

               // The aims of this update is to convert duration keywords and acquisition from "hh:mm:ss" format to a double of seconds

               // Create transaction if supported
               if (requester->transactionSupport())
                  requester->transactionBegin();

               //Step 1 : add column
               requester->addTableColumn(CAcquisitionSummaryTable::getTableName(), "count INTEGER NOT NULL default 1");

               CVersion_4_4_0::updateDatabaseVersion(requester, Version);

               // Commit transaction
               if (requester->transactionSupport())
                  requester->transactionCommit();
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.4.0 -> 5.0.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (requester->transactionSupport())
                  requester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 
