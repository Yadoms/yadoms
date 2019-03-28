#include "stdafx.h"
#include "Version_4_3_0.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/Version.h>
#include "VersionException.h"
#include <shared/Log.h>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "shared/plugin/yPluginApi/historization/Duration.h"
#include "database/common/adapters/MultipleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CVersion CVersion_4_3_0::Version(4, 3, 0);

         CVersion_4_3_0::CVersion_4_3_0()
         {
         }

         CVersion_4_3_0::~CVersion_4_3_0()
         {
         }

         void CVersion_4_3_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_4_2_0::checkForUpgrade(requester, currentVersion);

               //do update stuff
               updateFrom4_2_0(requester);
            }
            else
            {
               //good version
            }
         }

         void CVersion_4_3_0::updateFrom4_2_0(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.2.0 -> 4.3.0)...";

               // The aims of this update is to convert duration keywords and acquisition from "hh:mm:ss" format to a double of seconds

               // Create transaction if supported
               if (requester->transactionSupport())
                  requester->transactionBegin();

               auto findDurationKeywordsRequest = requester->newQuery();
               findDurationKeywordsRequest->Select().
                                            From(CKeywordTable::getTableName()).
                                            Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL,
                                                  shared::plugin::yPluginApi::CStandardCapacities::Duration().getName());
               adapters::CKeywordAdapter kwAdapter;
               requester->queryEntities(&kwAdapter, *findDurationKeywordsRequest);

               const auto results = kwAdapter.getResults();
               for (auto keyword = results.begin(); keyword != results.end(); ++keyword)
               {
                  YADOMS_LOG(debug) << "  Convert duration keyword #" << (*keyword)->Id() << " into double...";

                  // Update keyword entry in Keywords table (only unit and last acquisition columns impacted)
                  auto updateDurationKeywordRequest = requester->newQuery();
                  updateDurationKeywordRequest->Update(CKeywordTable::getTableName()).
                                                Set(CKeywordTable::getUnitsColumnName(),
                                                    shared::plugin::yPluginApi::CStandardCapacities::Duration().getUnit(),
                                                    CKeywordTable::getLastAcquisitionValueColumnName(),
                                                    convertDuration((*keyword)->LastAcquisitionValue())).
                                                Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, (*keyword)->Id());
                  requester->queryStatement(*updateDurationKeywordRequest);

                  // Update acquisitions (provided in "hh:mm:ss")
                  auto findDurationAcquisitionsRequest = requester->newQuery();
                  findDurationAcquisitionsRequest->Select().
                                                   From(CAcquisitionTable::getTableName()).
                                                   Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, (*keyword)->Id());
                  adapters::CAcquisitionAdapter acqAdapter;
                  requester->queryEntities(&acqAdapter, *findDurationAcquisitionsRequest);

                  const auto acquisitions = acqAdapter.getResults();
                  for (auto acquisition = acquisitions.begin(); acquisition != acquisitions.end(); ++acquisition)
                  {
                     auto updateAcquisitionRequest = requester->newQuery();
                     updateAcquisitionRequest->Update(CAcquisitionTable::getTableName()).
                                               Set(CAcquisitionTable::getValueColumnName(), convertDuration((*acquisition)->Value())).
                                               Where(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, (*acquisition)->Date());
                     requester->queryStatement(*updateAcquisitionRequest);
                  }

                  // Update acquisitions summary (provided in hours)
                  auto updateAcquisitionSummaryRequest = requester->newQuery();
                  // Use custom set for better performance (let do operation by the SQL engine)
                  updateAcquisitionSummaryRequest->Update(CAcquisitionSummaryTable::getTableName()).
                                                   Custom("SET  avgValue=avgValue*3600, minValue=minValue*3600, maxValue=maxValue*3600").
                                                   Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, (*keyword)->Id());
                  requester->queryStatement(*updateAcquisitionSummaryRequest);
               }

               CVersion_4_2_0::updateDatabaseVersion(requester, Version);

               // Commit transaction
               if (requester->transactionSupport())
                  requester->transactionCommit();
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.2.0 -> 4.3.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (requester->transactionSupport())
                  requester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }

         template <typename T>
         boost::optional<T> loadConfigurationValue(const boost::shared_ptr<IDatabaseRequester>& requester,
                                                   const std::string& section,
                                                   const std::string& name)
         {
            auto loadQuery = requester->newQuery();
            loadQuery->Select(CDatabaseColumn("value")).
                       From(CDatabaseTable("Configuration")).
                       Where(CDatabaseColumn("section"), CQUERY_OP_EQUAL, section).
                       And(CDatabaseColumn("name"), CQUERY_OP_EQUAL, name);

            adapters::CSingleValueAdapter<T> adapter;
            requester->queryEntities(&adapter, *loadQuery);
            return adapter.getResults().size() == 0 ? boost::optional<T>() : boost::optional<T>(adapter.getResults()[0]);
         }

         boost::optional<bool> CVersion_4_3_0::loadFirstStart(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<bool>(requester,
                                                "install",
                                                "firstStart");
         }

         boost::optional<std::string> CVersion_4_3_0::loadLocation(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "location");
         }

         boost::optional<std::string> CVersion_4_3_0::loadLanguage(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "language");
         }

         boost::optional<bool> CVersion_4_3_0::loadAdvancedParameters(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<bool>(requester,
                                                "system",
                                                "advancedParameters");
         }

         boost::optional<std::string> CVersion_4_3_0::loadDateFormatString(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "dateFormatString");
         }

         boost::optional<bool> CVersion_4_3_0::loadRefreshPage(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<bool>(requester,
                                                "system",
                                                "refreshPage");
         }

         boost::optional<std::string> CVersion_4_3_0::loadBasicAuthentication(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "basicAuthentication");
         }

         boost::optional<shared::CDataContainer> CVersion_4_3_0::convertLocation(const boost::optional<std::string>& oldLocation)
         {
            if (!oldLocation)
               return boost::optional<shared::CDataContainer>();

            boost::optional<shared::CDataContainer> oldLocationContainer(*oldLocation);
            shared::CDataContainer newLocation;

            newLocation.set("status", "userDefined");
            newLocation.set("latitude", oldLocationContainer->get<std::string>("latitude"));
            newLocation.set("longitude", oldLocationContainer->get<std::string>("longitude"));
            newLocation.set("altitude", oldLocationContainer->get<std::string>("altitude"));
            newLocation.set("timezone", oldLocationContainer->getWithDefault<std::string>("timezone", "Europe/Paris"));

            return newLocation;
         }

         boost::optional<shared::CDataContainer> CVersion_4_3_0::convertBasicAuthentication(
            const boost::optional<std::string>& oldBasicAuthentication)
         {
            if (!oldBasicAuthentication)
               return boost::optional<shared::CDataContainer>();
            return boost::optional<shared::CDataContainer>(*oldBasicAuthentication);
         }

         void CVersion_4_3_0::insertConfigurationValue(const boost::shared_ptr<IDatabaseRequester> requester,
                                                       const std::string& section,
                                                       const shared::CDataContainer& value,
                                                       const boost::posix_time::ptime& insertDate)
         {
            auto qInsert = requester->newQuery();
            qInsert->InsertOrReplaceInto(CConfigurationTable::getTableName(),
                                         CConfigurationTable::getSectionColumnName(),
                                         CConfigurationTable::getValueColumnName(),
                                         CConfigurationTable::getLastModificationDateColumnName()).
                     Values(section,
                            value.serialize(),
                            insertDate);
            requester->queryStatement(*qInsert);
         }

         std::string CVersion_4_3_0::convertDuration(const std::string& duration)
         {
            try
            {
               return std::to_string(boost::posix_time::duration_from_string(duration).total_microseconds() / 1000000.0);
            }
            catch (...)
            {
               return "0.0";
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 
