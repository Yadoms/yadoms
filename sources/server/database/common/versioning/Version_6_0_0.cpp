#include "stdafx.h"
#include "Version_6_0_0.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/SemVer.h>
#include "VersionException.h"
#include <shared/Log.h>

using namespace database::common::versioning;

// Modify this version to a greater value, to force update of current version
const shared::versioning::CSemVer CVersion_6_0_0::Version(6, 0, 0);

void CVersion_6_0_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                     const shared::versioning::CSemVer& currentVersion)
{
   if (currentVersion < Version)
   {
      //bad version, check base class version
      CVersion_5_2_0::checkForUpgrade(requester, currentVersion);

      //do update stuff
      updateFrom5_2_0(requester);
   }
   else
   {
      //good version
   }
}

// ReSharper disable once CppInconsistentNaming
void CVersion_6_0_0::updateFrom5_2_0(const boost::shared_ptr<IDatabaseRequester>& requester)
{
   try
   {
      YADOMS_LOG(information) << "Upgrading database (5.2.0 -> 6.0.0)...";

      // The aims of this update is to switch from Yadoms 2.5 to Yadoms 3.0 (new web API and client) :

      // Create transaction if supported
      if (requester->transactionSupport())
         requester->transactionBegin();

      //TODO Migration des widgets :
      // - traiter les configurations des widgets :
      //   - certaines données sont exprimées en string (exemple "checkbox": "false", "deviceId": "65", ...) ==> A remplacer par les types natifs

      //TODO mettre à jour CVersionUpgraderFactory::GetUpgrader pour pointer sur cette classe


      updateDatabaseVersion(requester, Version);

      // Commit transaction
      if (requester->transactionSupport())
         requester->transactionCommit();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(fatal) << "Failed to upgrade database (5.2.0 -> 6.0.0) : " << ex.what();
      YADOMS_LOG(fatal) << "Rollback transaction";
      if (requester->transactionSupport())
         requester->transactionRollback();
      throw CVersionException("Failed to update database");
   }
}
