#include "stdafx.h"
#include "UpdateManager.h"
#include <shared/Log.h>
#include "task/GenericTask.h"
#include "worker/Yadoms.h"
#include "worker/Plugin.h"
#include "worker/Widget.h"
#include "worker/ScriptInterpreter.h"
#include "info/UpdateSite.h"
#include "shared/tools/Filesystem.h"
#include "i18n/ClientStrings.h"


namespace update
{
   enum
   {
      kNextScanTimerId = shared::event::kUserFirstId,
      kStopNextScanTimerId,
      kStartNextScanTimerId
   };

   CUpdateManager::CUpdateManager(boost::shared_ptr<task::CScheduler>& taskScheduler,
                                  boost::shared_ptr<pluginSystem::CManager> pluginManager,
                                  boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                  bool developerMode,
                                  boost::shared_ptr<const IPathProvider> pathProvider)
      : m_taskScheduler(taskScheduler),
        m_pluginManager(pluginManager),
        m_eventLogger(eventLogger),
        m_developerMode(developerMode),
        m_pathProvider(pathProvider),
        m_thread(boost::thread(&CUpdateManager::doWork, this, boost::posix_time::hours(12)))
   {
   }

   CUpdateManager::~CUpdateManager()
   {
         m_thread.interrupt();
         m_thread.timed_join(boost::posix_time::seconds(30));
   }

   void CUpdateManager::doWork(const boost::posix_time::time_duration scanPeriod)
   {
      YADOMS_LOG_CONFIGURE("CUpdateManager");
      YADOMS_LOG(debug) << "Start";
      try
      {
         auto nexScanTimer = m_evtHandler.createTimer(kNextScanTimerId, shared::event::CEventTimer::kOneShot, scanPeriod);
         while (true)
         {
            switch (m_evtHandler.waitForEvents())
            {
            case kStopNextScanTimerId:
               {
                  nexScanTimer->stop();
                  break;
               }
            case kStartNextScanTimerId:
               {
                  nexScanTimer->start();
                  break;
               }
            case kNextScanTimerId:
               YADOMS_LOG(debug) << "Start scan...";
               if (scan())
                  notifyNewUpdateAvailable();
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

   bool CUpdateManager::scan()
   {
      auto updateAvailable = false;
      try
      {
         // Read inputs
         const auto pluginsLocalVersions = m_pluginManager->getPluginList();
         const auto pluginsAvailableVersions = info::CUpdateSite::getAllPluginVersions();

         const auto widgetsLocalVersions = worker::CWidget::getWidgetList();
         const auto widgetsAvailableVersions = info::CUpdateSite::getAllWidgetVersions();

         //TODO
         //const auto scriptInterpretersLocalVersions = getAvailableInterpreters();
         //const auto scriptInterpretersAvailableVersions = info::CUpdateSite::getAllScriptInterpreterVersions();


         const auto updates = buildUpdates(true,
                                           pluginsLocalVersions,
                                           pluginsAvailableVersions,
                                           widgetsLocalVersions,
                                           widgetsAvailableVersions);
         const auto releasesOnlyUpdates = buildUpdates(false,
                                                       pluginsLocalVersions,
                                                       pluginsAvailableVersions,
                                                       widgetsLocalVersions,
                                                       widgetsAvailableVersions);

         // Notify only for new releases (not for prereleases)
         if (releasesOnlyUpdates != m_releasesOnlyUpdates)
            updateAvailable = true;

         boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);
         m_allUpdates = updates;
         m_releasesOnlyUpdates = releasesOnlyUpdates;
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << " Error scanning available versions (do you have a working Internet connection ?), " << e
            .what();
         throw;
      }
      return updateAvailable;
   }

   void CUpdateManager::scanForUpdates(worker::CWorkerTools::WorkerProgressFunc progressCallback)
   {
      YADOMS_LOG(information) << "Scan for updates...";

      progressCallback(true, 0.0f, i18n::CClientStrings::ScanForUpdates, std::string(), shared::CDataContainer::EmptyContainer);

      // Suspend periodic updates scan
      m_evtHandler.postEvent(kStopNextScanTimerId);

      try
      {
         scan();
         progressCallback(true, 100.0f, i18n::CClientStrings::ScanForUpdatesSuccess, std::string(), shared::CDataContainer::EmptyContainer);
      }
      catch (std::exception&)
      {
         progressCallback(false, 100.0f, i18n::CClientStrings::ScanForUpdatesFailed, std::string(), shared::CDataContainer::EmptyContainer);
      }

      // Restart periodic updates scan
      m_evtHandler.postEvent(kStartNextScanTimerId);
   }

      shared::CDataContainer CUpdateManager::getUpdates(bool includePreleases) const
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);
         return includePreleases ? m_allUpdates : m_releasesOnlyUpdates;
      }

   shared::CDataContainer CUpdateManager::buildUpdates(bool includePreleases,
                                                       const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
                                                       const shared::CDataContainer& pluginsAvailableVersions,
                                                       const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
                                                       const shared::CDataContainer& widgetsAvailableVersions)
   {
      shared::CDataContainer updates;

      const auto pluginUpdates = buildPluginList(pluginsLocalVersions,
                                                 pluginsAvailableVersions,
                                                 includePreleases);
      updates.set("plugins", pluginUpdates);

      const auto widgetUpdates = buildWidgetList(widgetsLocalVersions,
                                                 widgetsAvailableVersions,
                                                 includePreleases);
      updates.set("widgets", widgetUpdates);

      return updates;
   }

   shared::CDataContainer CUpdateManager::buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                          const shared::CDataContainer& availableVersions,
                                                          bool includePreleases)
   {
      shared::CDataContainer list;

      // Add updatable items (ie already installed)
      const auto updatableItems = addUpdatablePlugins(localVersions,
                                                      availableVersions,
                                                      includePreleases);
      list.set("updatable", updatableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewPlugins(localVersions,
                                          availableVersions,
                                          includePreleases);
      list.set("new", newItems);

      return list;
   }

   shared::CDataContainer CUpdateManager::buildWidgetList(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                          const shared::CDataContainer& availableVersions,
                                                          bool includePreleases)
   {
      shared::CDataContainer list;

      // Add updatable items (ie already installed)
      const auto updatableItems = addUpdatableWidgets(localVersions,
                                                      availableVersions,
                                                      includePreleases);
      list.set("updatable", updatableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewWidgets(localVersions,
                                          availableVersions,
                                          includePreleases);
      list.set("new", newItems);

      return list;
   }

   shared::CDataContainer CUpdateManager::addUpdatablePlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                              const shared::CDataContainer& availableVersions,
                                                              bool includePreleases) const
   {
      shared::CDataContainer updatableItems;

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updatable modules
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            shared::CDataContainer item;

            item.set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

            shared::CDataContainer versions;
            versions.set("installed", localVersion.second->getVersion().toString());
            std::map<std::string, shared::CDataContainer> older; // Pass by a map to sort versions list
            std::map<std::string, shared::CDataContainer> newer; // Pass by a map to sort versions list
            if (availableVersions.exists(moduleType))
            {
               try
               {
                  const auto availableVersionsForPlugin = availableVersions.get<std::vector<shared::CDataContainer>>(moduleType);
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
                  YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
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
            }

            item.set("versions", versions);

            updatableItems.set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return updatableItems;
   }

   shared::CDataContainer CUpdateManager::addNewPlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                        const shared::CDataContainer& availableVersions,
                                                        bool includePreleases)
   {
      shared::CDataContainer newItems;

      for (const auto& moduleType : availableVersions.getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         // Module not installed
         const auto availableModule = availableVersions.get<shared::CDataContainer>(moduleType);

         // Pass by a map to sort versions list
         std::map<std::string, shared::CDataContainer> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForModule = availableVersions.get<std::vector<shared::CDataContainer>>(moduleType);
            for (auto& version : availableVersionsForModule)
            {
               shared::versioning::CVersion v(version.get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.prerelease().empty() && !includePreleases)
                  continue;

               shared::CDataContainer versionData;
               versionData.set("downloadUrl", version.get<std::string>("downloadUrl"));
               newModuleAvailableVersions[version.get<std::string>("version")] = versionData;
            }
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }


         if (newModuleAvailableVersions.empty())
            continue;


         // Create the versions node
         shared::CDataContainer versions;
         // Sort (newer version first)
         shared::CDataContainer sortedVersions;
         for (auto v = newModuleAvailableVersions.rbegin(); v != newModuleAvailableVersions.rend(); ++v)
            // Force different path char to not cut version string into subPaths
            sortedVersions.set(v->first, v->second, 0);
         versions.set("versions", sortedVersions);

         // Find the newest version
         shared::CDataContainer newestVersion;
         // Force different path char to not cut version string into subPaths
         newestVersion.set(newModuleAvailableVersions.rbegin()->first,
                           newModuleAvailableVersions.rbegin()->second, 0);
         versions.set("newest", newestVersion);


         // Add the module entry to the list
         try
         {
            shared::CDataContainer item;
            const auto& availableVersionsForPlugin = availableVersions.get<std::vector<shared::CDataContainer>>(moduleType);
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(availableVersionsForPlugin.begin(),
                                                         availableVersionsForPlugin.end(),
                                                         [&newestVersionLabel](
                                                         const shared::CDataContainer& availableVersionForPlugin)
                                                         {
                                                            return availableVersionForPlugin.get<std::string>(
                                                               "version") == newestVersionLabel;
                                                         });
            item.set("iconUrl", newestVersionData->get<std::string>("iconUrl"));
            item.set("versions", versions);

            newItems.set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return newItems;
   }

   shared::CDataContainer CUpdateManager::addUpdatableWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                              const shared::CDataContainer& availableVersions,
                                                              bool includePreleases) const
   {
      shared::CDataContainer updatableItems;

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updatable modules
            if (moduleType == "system-deactivated-widget")
               continue;
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            shared::CDataContainer item;

            // Widget path should be relative to www folder
            const auto widgetPath = shared::tools::CFilesystem::makeRelative(m_pathProvider->webServerPath(),
                                                                             localVersion.second->getPath());

            item.set("iconUrl", (widgetPath / "preview.png").generic_string());

            shared::CDataContainer versions;
            versions.set("installed", localVersion.second->getVersion().toString());
            std::map<std::string, shared::CDataContainer> older; // Pass by a map to sort versions list
            std::map<std::string, shared::CDataContainer> newer; // Pass by a map to sort versions list
            if (availableVersions.exists(moduleType))
            {
               try
               {
                  const auto availableVersionsForPlugin = availableVersions.get<std::vector<shared::CDataContainer>>(moduleType);
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
                  YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
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
            }

            item.set("versions", versions);

            updatableItems.set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return updatableItems;
   }

   shared::CDataContainer CUpdateManager::addNewWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                        const shared::CDataContainer& availableVersions,
                                                        bool includePreleases)
   {
      shared::CDataContainer newItems;

      for (const auto& moduleType : availableVersions.getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         // Module not installed
         const auto availableModule = availableVersions.get<shared::CDataContainer>(moduleType);

         // Pass by a map to sort versions list
         std::map<std::string, shared::CDataContainer> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForModule = availableVersions.get<std::vector<shared::CDataContainer>>(moduleType);
            for (auto& version : availableVersionsForModule)
            {
               shared::versioning::CVersion v(version.get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.prerelease().empty() && !includePreleases)
                  continue;

               shared::CDataContainer versionData;
               versionData.set("downloadUrl", version.get<std::string>("downloadUrl"));
               newModuleAvailableVersions[version.get<std::string>("version")] = versionData;
            }
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }


         if (newModuleAvailableVersions.empty())
            continue;


         // Create the versions node
         shared::CDataContainer versions;
         // Sort (newer version first)
         shared::CDataContainer sortedVersions;
         for (auto v = newModuleAvailableVersions.rbegin(); v != newModuleAvailableVersions.rend(); ++v)
            // Force different path char to not cut version string into subPaths
            sortedVersions.set(v->first, v->second, 0);
         versions.set("versions", sortedVersions);

         // Find the newest version
         shared::CDataContainer newestVersion;
         // Force different path char to not cut version string into subPaths
         newestVersion.set(newModuleAvailableVersions.rbegin()->first,
                           newModuleAvailableVersions.rbegin()->second, 0);
         versions.set("newest", newestVersion);


         // Add the module entry to the list
         try
         {
            shared::CDataContainer item;
            const auto& availableVersionsForPlugin = availableVersions.get<std::vector<shared::CDataContainer>>(moduleType);
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(availableVersionsForPlugin.begin(),
                                                         availableVersionsForPlugin.end(),
                                                         [&newestVersionLabel](
                                                         const shared::CDataContainer& availableVersionForPlugin)
                                                         {
                                                            return availableVersionForPlugin.get<std::string>(
                                                               "version") == newestVersionLabel;
                                                         });
            item.set("iconUrl", newestVersionData->get<std::string>("iconUrl"));
            item.set("versions", versions);

            newItems.set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return newItems;
   }

   void CUpdateManager::notifyNewUpdateAvailable() const
   {
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kUpdateAvailable, "yadoms", std::string());
   }

   bool CUpdateManager::startTask(boost::shared_ptr<task::ITask> task,
                                  std::string& taskUid) const
   {
      auto result = false;
      if (task)
      {
         result = m_taskScheduler->runTask(task, taskUid);
         if (result)
            YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;
         else
            YADOMS_LOG(error) << "Task : " << task->getName() << " fail to start";
      }
      else
      {
         YADOMS_LOG(error) << "The task is not known";
      }
      return result;
   }

   std::string CUpdateManager::startTask(boost::shared_ptr<task::ITask> task) const
   {
      std::string taskId = "";
      if (startTask(task, taskId))
         return taskId;
      throw shared::exception::CException("Fail to start task");
   }


   std::string CUpdateManager::scanForUpdatesAsync()
   {
      const auto task(boost::make_shared<task::CGenericTask>("scanForUpdates",
                                                             boost::bind(&CUpdateManager::scanForUpdates, this, _1)));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::updatePluginAsync(const std::string& pluginName,
                                                 const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("plugin.update",
                                                             boost::bind(&worker::CPlugin::update, _1, std::string(pluginName),
                                                                         std::string(downloadUrl), m_pluginManager)));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::installPluginAsync(const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("plugin.install",
                                                             boost::bind(&worker::CPlugin::install, _1, std::string(downloadUrl),
                                                                         m_pluginManager)));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::removePluginAsync(const std::string& pluginName) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("plugin.remove",
                                                             boost::bind(&worker::CPlugin::remove, _1, std::string(pluginName),
                                                                         m_pluginManager)));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::updateWidgetAsync(const std::string& widgetName,
                                                 const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("widget.update",
                                                             boost::bind(&worker::CWidget::update, _1, std::string(widgetName),
                                                                         std::string(downloadUrl))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::installWidgetAsync(const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("widget.install",
                                                             boost::bind(&worker::CWidget::install, _1, std::string(downloadUrl))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::removeWidgetAsync(const std::string& widgetName) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("widget.remove",
                                                             boost::bind(&worker::CWidget::remove, _1, std::string(widgetName))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }


   std::string CUpdateManager::updateScriptInterpreterAsync(const std::string& scriptInterpreterName,
                                                            const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("scriptInterpreter.update",
                                                             boost::bind(&worker::CScriptInterpreter::update, _1,
                                                                         std::string(scriptInterpreterName),
                                                                         std::string(downloadUrl))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::installScriptInterpreterAsync(const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("scriptInterpreter.install",
                                                             boost::bind(&worker::CScriptInterpreter::install, _1,
                                                                         std::string(downloadUrl))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("scriptInterpreter.remove",
                                                             boost::bind(&worker::CScriptInterpreter::install, _1,
                                                                         std::string(scriptInterpreterName))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::updateYadomsAsync(const shared::CDataContainer& versionToInstall) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("yadoms.update",
                                                             boost::bind(&worker::CYadoms::update, _1,
                                                                         shared::CDataContainer(versionToInstall))));
      //force to copy parameter because the versionToInstall is a reference and cannot be used "as is" in async task
      return startTask(task);
   }
} // namespace update
