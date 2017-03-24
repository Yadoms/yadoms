#include "stdafx.h"
#include "Version_3_0_1.h"
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
         const shared::versioning::CVersion CVersion_3_0_1::Version(3, 0, 1);

         CVersion_3_0_1::CVersion_3_0_1()
         {
         }

         CVersion_3_0_1::~CVersion_3_0_1()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_3_0_1::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_3_0_0::checkForUpgrade(pRequester, currentVersion);

               //do update stuff
               UpdateFrom3_0_0(pRequester);
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
         void CVersion_3_0_1::UpdateFrom3_0_0(const boost::shared_ptr<IDatabaseRequester>& pRequester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (3.0.0 -> 3.0.1)";

               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               auto qUpdate = pRequester->newQuery();
               qUpdate.Update(CRuleTable::getTableName()).
                      Set(CRuleTable::getInterpreterColumnName(), "yPython27").
                      Where(CRuleTable::getInterpreterColumnName(), CQUERY_OP_EQUAL, "Python");

               pRequester->queryStatement(qUpdate);

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
               YADOMS_LOG(fatal) << "Failed to upgrade database (3.0.0 -> 3.0.1) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


