#include "stdafx.h"
#include "Version_4_2_0.h"
#include "database/common/Query.h"
#include "database/common/DatabaseTables.h"
#include <shared/versioning/Version.h>
#include "VersionException.h"
#include <shared/Log.h>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "shared/currentTime/Provider.h"

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CVersion CVersion_4_2_0::Version(4, 2, 0);

         CVersion_4_2_0::CVersion_4_2_0()
         {
         }

         CVersion_4_2_0::~CVersion_4_2_0()
         {
         }

         void CVersion_4_2_0::checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            if (currentVersion < Version)
            {
               //bad version, check base class version
               CVersion_4_1_0::checkForUpgrade(requester, currentVersion);

               //do update stuff
               updateFrom4_1_0(requester);
            }
            else
            {
               //good version
            }
         }

         void CVersion_4_2_0::updateFrom4_1_0(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            try
            {
               YADOMS_LOG(information) << "Upgrading database (4.1.0 -> 4.2.0)...";
               const auto insertDate = shared::currentTime::Provider().now();

               // The aims of this update is to reorganize configuration.
               // We want now to have :
               //   - a record containing the server configuration (configuration associated to the server)
               //   - a record by GUI kind

               // The table "Configuration" is empty (except databaseVersion record) if it is the first server start
               auto qSelect = requester->newQuery();
               qSelect->SelectCount().
                        From(CConfigurationTable::getTableName()).
                        Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_NOT_EQUAL, "databaseVersion");
               if (requester->queryCount(*qSelect) != 0)
               {
                  // First load actual values
                  const auto firstStart = loadFirstStart(requester);
                  const auto location = convertLocation(loadLocation(requester));
                  const auto language = loadLanguage(requester);
                  const auto advancedParameters = loadAdvancedParameters(requester);
                  const auto dateFormatString = loadDateFormatString(requester);
                  const auto refreshPage = loadRefreshPage(requester);
                  const auto basicAuthentication = convertBasicAuthentication(loadBasicAuthentication(requester));

                  // Reorganize
                  // - The server configuration
                  shared::CDataContainer serverConfiguration;
                  serverConfiguration.set("firstStart", firstStart);
                  serverConfiguration.set("location", location);
                  serverConfiguration.set("basicAuthentication", basicAuthentication);
                  // - The GUI configuration (actually only one known GUI client kind)
                  shared::CDataContainer webClientConfiguration;
                  webClientConfiguration.set("language", language);
                  webClientConfiguration.set("advancedParametersActive", advancedParameters);
                  webClientConfiguration.set("dateFormatString", dateFormatString);
                  webClientConfiguration.set("refreshPage", refreshPage);


                  // Refactor the configuration table
                  if (requester->transactionSupport())
                     requester->transactionBegin();

                  // Remove current Configuration table
                  if (!requester->dropTableIfExists(CDatabaseTable("Configuration")))
                     throw std::runtime_error("Fail to drop Configuration table");

                  // Create new Configuration table
                  requester->createTableIfNotExists(CConfigurationTable::getTableName(),
                                                    requester->getTableCreationScriptProvider()->getTableConfiguration());

                  // Insert new records
                  insertConfigurationValue(requester, "server", serverConfiguration, insertDate);
                  insertConfigurationValue(requester, "external.webClient", webClientConfiguration, insertDate);
               }

               updateDatabaseVersion(requester, Version, insertDate);

               // Commit transaction
               if (requester->transactionSupport())
                  requester->transactionCommit();

               // Compact database
               requester->vacuum();
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database (4.1.0 -> 4.2.0) : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (requester->transactionSupport())
                  requester->transactionRollback();
               throw CVersionException("Failed to update database");
            }
         }

         template <typename T>
         T loadConfigurationValue(const boost::shared_ptr<IDatabaseRequester>& requester,
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
            return adapter.getResults()[0];
         }

         bool CVersion_4_2_0::loadFirstStart(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<bool>(requester,
                                                "install",
                                                "firstStart");
         }

         std::string CVersion_4_2_0::loadLocation(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "location");
         }

         std::string CVersion_4_2_0::loadLanguage(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "language");
         }

         bool CVersion_4_2_0::loadAdvancedParameters(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<bool>(requester,
                                                "system",
                                                "advancedParameters");
         }

         std::string CVersion_4_2_0::loadDateFormatString(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "dateFormatString");
         }

         bool CVersion_4_2_0::loadRefreshPage(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<bool>(requester,
                                                "system",
                                                "refreshPage");
         }

         std::string CVersion_4_2_0::loadBasicAuthentication(const boost::shared_ptr<IDatabaseRequester>& requester)
         {
            return loadConfigurationValue<std::string>(requester,
                                                       "system",
                                                       "basicAuthentication");
         }

         shared::CDataContainer CVersion_4_2_0::convertLocation(const std::string& oldLocation)
         {
            shared::CDataContainer oldLocationContainer(oldLocation);
            shared::CDataContainer newLocation;

            newLocation.set("location.status", "userDefined");
            newLocation.set("latitude", oldLocationContainer.get<std::string>("latitude"));
            newLocation.set("longitude", oldLocationContainer.get<std::string>("longitude"));
            newLocation.set("altitude", oldLocationContainer.get<std::string>("altitude"));
            newLocation.set("timezone", oldLocationContainer.get<std::string>("timezone"));

            return newLocation;
         }

         shared::CDataContainer CVersion_4_2_0::convertBasicAuthentication(const std::string& oldBasicAuthentication)
         {
            return shared::CDataContainer(oldBasicAuthentication);
         }

         void CVersion_4_2_0::insertConfigurationValue(const boost::shared_ptr<IDatabaseRequester> requester,
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
      } //namespace versioning
   } //namespace common
} //namespace database 
