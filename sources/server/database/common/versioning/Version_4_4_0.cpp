#include "stdafx.h"
#include "Version_4_4_0.h"
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
         const shared::versioning::CVersion CVersion_4_4_0::Version(4, 4, 0);

         CVersion_4_4_0::CVersion_4_4_0()
         {
         }

         CVersion_4_4_0::~CVersion_4_4_0()
         {
         }

         void CVersion_4_4_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_4_3_0::checkForUpgrade(requester, currentVersion);

               //do update stuff
               updateFrom4_3_0(requester);
            }
            else
            {
               //good version
            }
         }

         void CVersion_4_4_0::updateFrom4_3_0(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.3.0 -> 4.4.0)...";

               // The aims of this update is to convert duration keywords and acquisition from "hh:mm:ss" format to a double of seconds

               // Create transaction if supported
               if (requester->transactionSupport())
                  requester->transactionBegin();

               /////////////////////////////////////////
               //due to problems with computing summary acquisition for months and years, then compute all
               /////////////////////////////////////////

               //Step 1 : erase all Month and Year summaries
               auto qeraseSummaries = requester->newQuery();
               qeraseSummaries->DeleteFrom(CAcquisitionSummaryTable::getTableName()).
                  Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, entities::EAcquisitionSummaryType::kMonth)
                  .Or(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, entities::EAcquisitionSummaryType::kYear);
               requester->queryStatement(*qeraseSummaries);


               //step 2 : compute all months values
               auto qMonthSummaries = requester->newQuery();
               //qMonthSummaries->fromSubquery(CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getTableName())
               qMonthSummaries->InsertInto(CAcquisitionSummaryTable::getTableName(),
                  CAcquisitionSummaryTable::getTypeColumnName(),
                  CAcquisitionSummaryTable::getDateColumnName(),
                  CAcquisitionSummaryTable::getKeywordIdColumnName(),
                  CAcquisitionSummaryTable::getAvgColumnName(),
                  CAcquisitionSummaryTable::getMinColumnName(),
                  CAcquisitionSummaryTable::getMaxColumnName()).
                  Select(entities::EAcquisitionSummaryType::kMonth,
                     qMonthSummaries->as(qMonthSummaries->concatenate(qMonthSummaries->substr(CAcquisitionSummaryTable::getDateColumnName(), 0, 6), "01T000000"), "isodate"),
                     CAcquisitionSummaryTable::getKeywordIdColumnName(),
                     qMonthSummaries->average(CAcquisitionSummaryTable::getAvgColumnName()),
                     qMonthSummaries->min(CAcquisitionSummaryTable::getMinColumnName()),
                     qMonthSummaries->max(CAcquisitionSummaryTable::getMaxColumnName())).
                  From(CAcquisitionSummaryTable::getTableName()).
                  Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, entities::EAcquisitionSummaryType::kDay).
                  GroupBy(CAcquisitionSummaryTable::getKeywordIdColumnName(),  "isodate");
               
               requester->queryStatement(*qMonthSummaries);

               
               //step 3 : comptue all years values
               auto qYearSummaries = requester->newQuery();

               qYearSummaries->InsertInto(CAcquisitionSummaryTable::getTableName(),
                  CAcquisitionSummaryTable::getTypeColumnName(),
                  CAcquisitionSummaryTable::getDateColumnName(),
                  CAcquisitionSummaryTable::getKeywordIdColumnName(),
                  CAcquisitionSummaryTable::getAvgColumnName(),
                  CAcquisitionSummaryTable::getMinColumnName(),
                  CAcquisitionSummaryTable::getMaxColumnName()).
                  Select(entities::EAcquisitionSummaryType::kYear,
                     qYearSummaries->as(qYearSummaries->concatenate(qYearSummaries->substr(CAcquisitionSummaryTable::getDateColumnName(), 0, 4), "0101T000000"), "isodate"),
                     CAcquisitionSummaryTable::getKeywordIdColumnName(),
                     qYearSummaries->average(CAcquisitionSummaryTable::getAvgColumnName()),
                     qYearSummaries->min(CAcquisitionSummaryTable::getMinColumnName()),
                     qYearSummaries->max(CAcquisitionSummaryTable::getMaxColumnName())).
                  From(CAcquisitionSummaryTable::getTableName()).
                  Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, entities::EAcquisitionSummaryType::kMonth).
                  GroupBy(CAcquisitionSummaryTable::getKeywordIdColumnName(), "isodate");

               requester->queryStatement(*qYearSummaries);

               CVersion_4_3_0::updateDatabaseVersion(requester, Version);

               // Commit transaction
               if (requester->transactionSupport())
                  requester->transactionCommit();
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.3.0 -> 4.4.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (requester->transactionSupport())
                  requester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 
