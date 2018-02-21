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

               // The aims of this update is to reorganize configuration.
               // We want now to have :
               //   - a record containing the server configuration (configuration associated to the server)
               //   - a record by GUI kind

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
               shared::CDataContainer guiConfiguration;
               guiConfiguration.set("language", language);
               guiConfiguration.set("advancedParametersActive", advancedParameters);
               // Boolean values were recorded as strings in old database
               guiConfiguration.set("dateFormatString", dateFormatString);
               guiConfiguration.set("refreshPage", refreshPage);


               // Create the new configuration table (keep the old in case of use downgrades his Yadoms version)
               if (requester->transactionSupport())
                  requester->transactionBegin();

               requester->createTableIfNotExists(CConfigurationTable::getTableName(),
                                                 requester->getTableCreationScriptProvider()->getTableConfiguration());

               const auto insertDate = shared::currentTime::Provider().now();

               insertConfigurationValue(requester, "server", serverConfiguration, insertDate);
               insertConfigurationValue(requester, "webClient", guiConfiguration, insertDate);

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
            loadQuery->Select("value").
                       From("Configuration").
                       Where("section", CQUERY_OP_EQUAL, section).
                       And("name", CQUERY_OP_EQUAL, name);

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
            return shared::CDataContainer(oldLocation);
            //TODO faut-il passer les valeurs en type natif (actuellement en string), attention aux arrondis ?
         }

         shared::CDataContainer CVersion_4_2_0::convertBasicAuthentication(const std::string& oldBasicAuthentication)
         {
            shared::CDataContainer oldBasicAuthenticationContainer(oldBasicAuthentication);
            shared::CDataContainer newBasicAuthentication;

            // Boolean values were recorded as strings in old database
            newBasicAuthentication.set("active", oldBasicAuthenticationContainer.get<std::string>("active") == "true");
            newBasicAuthentication.set("user", oldBasicAuthenticationContainer.get<std::string>("user"));
            newBasicAuthentication.set("password", oldBasicAuthenticationContainer.get<std::string>("password"));

            return newBasicAuthentication;
         }

         void CVersion_4_2_0::updateDatabaseVersion(const boost::shared_ptr<IDatabaseRequester> requester,
                                                    const shared::versioning::CVersion& newVersion,
                                                    const boost::posix_time::ptime& insertDate)
         {
            auto qInsert = requester->newQuery();
            qInsert->InsertOrReplaceInto(CConfigurationTable::getTableName(),
                                         CConfigurationTable::getSectionColumnName(),
                                         CConfigurationTable::getValueColumnName(),
                                         CConfigurationTable::getLastModificationDateColumnName()).
                     Values("DatabaseVersion",
                            Version.toString(),
                            insertDate);
            requester->queryStatement(*qInsert);
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
