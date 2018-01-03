#include "stdafx.h"
#include "UpdateChecker.h"
#include "shared/Log.h"
#include "update/info/UpdateSite.h"

namespace update
{
   namespace worker
   {
      enum
      {
         kNextScanTimerId = shared::event::kUserFirstId,
         kForceScanEventId
      };


      CUpdateChecker::CUpdateChecker(const boost::posix_time::time_duration firstScanDelay,
                                     const boost::posix_time::time_duration nextScanDelays,
                                     boost::shared_ptr<pluginSystem::CManager> pluginManager,
                                     boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger)
         : m_pluginManager(pluginManager),
           m_eventLogger(eventLogger),
           m_thread(boost::thread(&CUpdateChecker::doWork, this, firstScanDelay, nextScanDelays)),
           m_updatesAvailable(false)
      {
      }

      CUpdateChecker::~CUpdateChecker()
      {
         m_thread.interrupt();
         m_thread.timed_join(boost::posix_time::seconds(30));
      }

      void CUpdateChecker::forceRebuildUpdates()
      {
         m_evtHandler.postEvent(kForceScanEventId);
      }

      shared::CDataContainer CUpdateChecker::getUpdates(bool includePreleases) const
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);
         return includePreleases ? m_allUpdates : m_releasesOnlyUpdates;
      }

      void CUpdateChecker::doWork(const boost::posix_time::time_duration firstScanDelay,
                                  const boost::posix_time::time_duration nextScanDelays)
      {
         YADOMS_LOG_CONFIGURE("CUpdateChecker");
         YADOMS_LOG(debug) << "Start";
         try
         {
            // Wait for first scan
            //TODO remettre
            //switch (m_evtHandler.waitForEvents(firstScanDelay))
            //{
            //case shared::event::kTimeout:
            //   break;

            //case kForceScanEventId:
            //   YADOMS_LOG(debug) << "Force first scan...";
            //   break;

            //default:
            //   YADOMS_LOG(error) << "Unsupported event " << m_evtHandler.getEventId() << ", ignored";
            //   break;
            //}

            YADOMS_LOG(debug) << "Start first scan...";
            scan();

            auto nexScanTimer = m_evtHandler.createTimer(kNextScanTimerId, shared::event::CEventTimer::kOneShot,
                                                         nextScanDelays);
            while (true)
            {
               switch (m_evtHandler.waitForEvents())
               {
               case kNextScanTimerId:
                  YADOMS_LOG(debug) << "Start scan...";
                  scan();
                  break;

               case kForceScanEventId:
                  YADOMS_LOG(debug) << "Force scan...";
                  nexScanTimer->stop();
                  scan();
                  nexScanTimer->start();
                  break;

               default:
                  YADOMS_LOG(error) << "Unsupported event " << m_evtHandler.getEventId() << ", ignored";
                  break;
               }
            }
         }
         catch (boost::thread_interrupted&)
         {
            YADOMS_LOG(debug) << "Interruption";
         }
      }

      void CUpdateChecker::scan()
      {
         m_updatesAvailable = false;
         const auto updates = buildUpdates(true /*TODO à gérer*/);
         const auto releasesOnlyUpdates = extractReleases(updates);

         YADOMS_LOG(debug) << "updates : ";//TODO virer
         updates.printToLog(YADOMS_LOG(debug));
         YADOMS_LOG(debug) << "releasesOnlyUpdates : ";//TODO virer
         releasesOnlyUpdates.printToLog(YADOMS_LOG(debug));

         // Notify only for new releases (not for prereleases)
         if (releasesOnlyUpdates != m_releasesOnlyUpdates)
            m_eventLogger->addEvent(database::entities::ESystemEventCode::kUpdateAvailable, "yadoms", std::string());

         boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);
         m_allUpdates = updates;
         m_releasesOnlyUpdates = releasesOnlyUpdates;
      }

      shared::CDataContainer CUpdateChecker::buildUpdates(bool includePreleases)
      {
         shared::CDataContainer updates;

         const auto pluginsLocalVersions = m_pluginManager->getPluginList();
         const auto pluginsAvailableVersions = info::CUpdateSite::getAllPluginVersions("fr" /*TODO à gérer*/);
         const auto pluginUpdates = buildPluginList(pluginsLocalVersions,
                                                    pluginsAvailableVersions,
                                                    includePreleases);

         updates.set("plugins", pluginUpdates);

         return updates;
      }

      shared::CDataContainer CUpdateChecker::buildPluginList(
         const pluginSystem::IFactory::AvailablePluginMap& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePreleases)
      {
         shared::CDataContainer list;
         for (const auto& localVersion : localVersions)
         {
            try
            {
               const auto pluginType = localVersion.first;
               shared::CDataContainer item;

               //TODO conserver ? Le client doit pouvoir le récupérer comme le name/description
               item.set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

               shared::CDataContainer versions;
               versions.set("installed", localVersion.second->getVersion().toString());
               std::map<std::string, shared::CDataContainer> older; // Pass by a map to sort versions list
               std::map<std::string, shared::CDataContainer> newer; // Pass by a map to sort versions list
               if (availableVersions.exists(pluginType))
               {
                  try
                  {
                     const auto availableVersionsForPlugin =
                        availableVersions.get<std::vector<shared::CDataContainer>>(pluginType);
                     for (auto& version : availableVersionsForPlugin)
                     {
                        shared::versioning::CVersion v(version.get<std::string>("version"));

                        // Don't add prereleases versions if not asked
                        if (!v.prerelease().empty() && !includePreleases)
                           continue;

                        if (v == localVersion.second->getVersion())
                           continue;

                        shared::CDataContainer versionData;
                        versionData.set("downloadUrl", version.get<std::string>("downloadUrl"));

                        if (v < localVersion.second->getVersion())
                           older[version.get<std::string>("version")] = versionData;
                        else
                           newer[version.get<std::string>("version")] = versionData;
                     }
                  }
                  catch (std::exception& exception)
                  {
                     YADOMS_LOG(warning) << "Invalid remote package for " << pluginType << ", will be ignored";
                     YADOMS_LOG(debug) << "exception : " << exception.what();
                  }
               }

               if (!older.empty())
               {
                  // Sort (newer version first)
                  shared::CDataContainer olderVersions;
                  for (auto v = older.rbegin(); v != older.rend(); ++v)
                     // Force different path char to not cut version string into subPaths
                     olderVersions.set(v->first, v->second, 0);
                  versions.set("older", olderVersions);
               }

               if (!newer.empty())
               {
                  // Sort (newer version first)
                  shared::CDataContainer newerVersions;
                  for (auto v = newer.rbegin(); v != newer.rend(); ++v)
                     // Force different path char to not cut version string into subPaths
                     newerVersions.set(v->first, v->second, 0);
                  versions.set("newer", newerVersions);

                  // Find the newest version
                  shared::CDataContainer newestVersion;
                  // Force different path char to not cut version string into subPaths
                  newestVersion.set(newer.rbegin()->first, newer.rbegin()->second, 0);
                  versions.set("newest", newestVersion);

                  m_updatesAvailable = true;
               }

               item.set("versions", versions);

               list.set(pluginType, item);
            }
            catch (std::exception& exception)
            {
               YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
               YADOMS_LOG(debug) << "exception : " << exception.what();
            }
         }

         // Add items not already installed (ie not in localVersions list)
         for (const auto& pluginType : availableVersions.getKeys())
         {
            if (localVersions.find(pluginType) != localVersions.end())
               continue; // Module already installed

            // Module not installed
            const auto availableModule = availableVersions.get<shared::CDataContainer>(pluginType);

            // Pass by a map to sort versions list
            std::map<std::string, shared::CDataContainer> newPluginAvailableVersions;
            try
            {
               const auto availableVersionsForPlugin =
                  availableVersions.get<std::vector<shared::CDataContainer>>(pluginType);
               for (auto& version : availableVersionsForPlugin)
               {
                  shared::versioning::CVersion v(version.get<std::string>("version"));

                  // Don't add prereleases versions if not asked
                  if (!v.prerelease().empty() && !includePreleases)
                     continue;

                  shared::CDataContainer versionData;
                  versionData.set("downloadUrl", version.get<std::string>("downloadUrl"));
                  newPluginAvailableVersions[version.get<std::string>("version")] = versionData;
               }
            }
            catch (std::exception& exception)
            {
               YADOMS_LOG(warning) << "Invalid remote package for " << pluginType << ", will be ignored";
               YADOMS_LOG(debug) << "exception : " << exception.what();
            }


            if (newPluginAvailableVersions.empty())
               continue;

            shared::CDataContainer versions;
            // Sort (newer version first)
            shared::CDataContainer sortedVersions;
            for (auto v = newPluginAvailableVersions.rbegin(); v != newPluginAvailableVersions.rend(); ++v)
               // Force different path char to not cut version string into subPaths
               sortedVersions.set(v->first, v->second, 0);
            versions.set("versions", sortedVersions);

            // Find the newest version
            shared::CDataContainer newestVersion;
            // Force different path char to not cut version string into subPaths
            newestVersion.set(newPluginAvailableVersions.rbegin()->first,
                              newPluginAvailableVersions.rbegin()->second, 0);
            versions.set("newest", newestVersion);

            try
            {
               shared::CDataContainer item;
               const auto& availableVersionsForPlugin = availableVersions.get<std::vector<shared::CDataContainer>>(
                  pluginType);
               const auto& newestVersionLabel = newPluginAvailableVersions.rbegin()->first;
               const auto& newestVersionData = std::find_if(availableVersionsForPlugin.begin(),
                                                            availableVersionsForPlugin.end(),
                                                            [&newestVersionLabel](
                                                            const shared::CDataContainer& availableVersionForPlugin)
                                                            {
                                                               return availableVersionForPlugin.get<std::string>(
                                                                  "version") == newestVersionLabel;
                                                            });
               newestVersionData->printToLog(YADOMS_LOG(debug));
               item.set("iconUrl", newestVersionData->get<std::string>("iconUrl"));
               item.set("versions", versions);

               list.set(pluginType, item);
            }
            catch (std::exception& exception)
            {
               YADOMS_LOG(warning) << "Invalid remote package for " << pluginType << ", will be ignored";
               YADOMS_LOG(debug) << "exception : " << exception.what();
            }
         }

         return list;
      }

      shared::CDataContainer CUpdateChecker::extractReleases(const shared::CDataContainer& inUpdates)
      {
         shared::CDataContainer outUpdates;
         for (const auto& inModuleTypeNode : inUpdates.getKeys()) // inModuleTypeNode = ["plugins", "widgets" ...]
         {
            shared::CDataContainer outModuleType;
            auto modules = inUpdates.get<shared::CDataContainer>(inModuleTypeNode);
            for (const auto& inModuleKeyNode : modules.getKeys()) // inModuleTypeNode = ["rfxcom", "Sigfox" ...]
            {
               shared::CDataContainer outModuleKey;
               auto inModule = modules.get<shared::CDataContainer>(inModuleKeyNode);
               for (const auto& inItem : inModule.getKeys()) // inModuleTypeNode = ["iconUrl", "versions" ...]
               {
                  if (inItem != "versions")
                  {
                     if (inModule.containsValue(inItem))
                        outModuleKey.set(inItem, inModule.get<std::string>(inItem));
                     else
                        outModuleKey.set(inItem, inModule.get<shared::CDataContainer>(inItem));

                     continue;
                  }

                  auto inModuleVersions = inModule.get<shared::CDataContainer>("versions");
                  shared::CDataContainer outModuleVersions;

                  if (inModuleVersions.exists("installed"))
                     outModuleVersions.set("installed", inModuleVersions.get<std::string>("installed"));

                  if (inModuleVersions.exists("older"))
                  {
                     const auto olderVersions = inModuleVersions.get<shared::CDataContainer>("older");
                     if (!olderVersions.empty())
                     {
                        const auto olderReleases = extractReleasesFromVersionsNode(olderVersions);
                        if (!olderReleases.empty())
                           outModuleVersions.set("older", olderReleases);
                     }
                  }

                  if (inModuleVersions.exists("newer"))
                  {
                     const auto newerVersions = inModuleVersions.get<shared::CDataContainer>("newer");
                     if (!newerVersions.empty())
                     {
                        const auto newerReleases = extractReleasesFromVersionsNode(newerVersions);
                        if (!newerReleases.empty())
                        {
                           outModuleVersions.set("newer", newerReleases);
                           const auto& newerReleasesKeys = newerReleases.getKeys();
                           shared::CDataContainer newest;
                           newest.set(newerReleasesKeys.front(), newerReleases.get<shared::CDataContainer>(newerReleasesKeys.front(), 0), 0);
                           outModuleVersions.set("newest", newest);                           
                        }
                     }
                  }

                  outModuleKey.set("versions", outModuleVersions);
               }
               outModuleType.set(inModuleKeyNode, outModuleKey);
            }
            outUpdates.set(inModuleTypeNode, outModuleType);
         }

         return outUpdates;
      }

      shared::CDataContainer CUpdateChecker::extractReleasesFromVersionsNode(
         const shared::CDataContainer& inVersionsNode)
      {
         shared::CDataContainer out;
         for (const auto& inVersionNode : inVersionsNode.getKeys())
         {
            if (shared::versioning::CVersion(inVersionNode).prerelease().empty())
               out.set(inVersionNode, inVersionsNode.get<shared::CDataContainer>(inVersionNode, 0), 0);
         }

         return out;
      }
   } // namespace worker
} // namespace update
