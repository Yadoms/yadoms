#include "stdafx.h"
#include "UpdateManager.h"
#include <shared/Log.h>
#include "task/GenericTask.h"
#include "worker/Yadoms.h"
#include "worker/Plugin.h"
#include "worker/Widget.h"
#include "worker/ScriptInterpreter.h"
#include "info/UpdateSite.h"
#include <shared/tools/Filesystem.h>
#include "i18n/ClientStrings.h"
#include <shared/ServiceLocator.h>


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
                                  boost::shared_ptr<automation::interpreter::IManager> interpreterManager,
                                  boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                  bool developerMode,
                                  boost::shared_ptr<const IPathProvider> pathProvider)
      : m_taskScheduler(taskScheduler),
        m_pluginManager(pluginManager),
        m_interpreterManager(interpreterManager),
        m_eventLogger(eventLogger),
        m_developerMode(developerMode),
        m_pathProvider(pathProvider),
        m_allUpdates(new_CDataContainerSharedPtr()),
        m_releasesOnlyUpdates(new_CDataContainerSharedPtr()),
        m_thread(boost::thread(&CUpdateManager::doWork, this, boost::posix_time::hours(12)))
   {
   }

   CUpdateManager::~CUpdateManager()
   {
      m_thread.interrupt();
      m_thread.timed_join(boost::posix_time::seconds(30));
   }

   void CUpdateManager::doWork(const boost::posix_time::time_duration& scanPeriod)
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
               try
               {
                  if (scan(m_pathProvider))
                     notifyNewUpdateAvailable();
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Unable to check for Update, " << exception.what();
               }
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

   bool CUpdateManager::scan(const boost::shared_ptr<const IPathProvider> pathProvider)
   {
      auto updateAvailable = false;
      try
      {
         // Read inputs
         YADOMS_LOG(debug) << "Read Yadoms versions...";
         const auto yadomsLocalVersion = shared::CServiceLocator::instance().get<IRunningInformation>()->getSoftwareVersion().getVersion();
         const auto yadomsAvailableVersions = info::CUpdateSite::getAllYadomsVersions();

         YADOMS_LOG(debug) << "Read Plugins versions...";
         const auto pluginsLocalVersions = m_pluginManager->getPluginList();
         const auto pluginsAvailableVersions = info::CUpdateSite::getAllPluginVersions();

         YADOMS_LOG(debug) << "Read Widgets versions...";
         const auto widgetsLocalVersions = worker::CWidget::getWidgetList(pathProvider->widgetsPath());
         const auto widgetsAvailableVersions = info::CUpdateSite::getAllWidgetVersions();

         YADOMS_LOG(debug) << "Read ScriptInterpreters versions...";
         const auto scriptInterpretersLocalVersions = m_interpreterManager->getLoadedInterpretersInformation();
         const auto scriptInterpretersAvailableVersions = info::CUpdateSite::getAllScriptInterpreterVersions();

         YADOMS_LOG(debug) << "Build updates data (with prereleases)...";
         const auto updates = buildUpdates(true,
                                           yadomsLocalVersion,
                                           yadomsAvailableVersions,
                                           pluginsLocalVersions,
                                           pluginsAvailableVersions,
                                           widgetsLocalVersions,
                                           widgetsAvailableVersions,
                                           scriptInterpretersLocalVersions,
                                           scriptInterpretersAvailableVersions);

         YADOMS_LOG(debug) << "Build updates data (releases only)...";
         const auto releasesOnlyUpdates = buildUpdates(false,
                                                       yadomsLocalVersion,
                                                       yadomsAvailableVersions,
                                                       pluginsLocalVersions,
                                                       pluginsAvailableVersions,
                                                       widgetsLocalVersions,
                                                       widgetsAvailableVersions,
                                                       scriptInterpretersLocalVersions,
                                                       scriptInterpretersAvailableVersions);

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

   void CUpdateManager::scanForUpdates(const worker::CWorkerTools::WorkerProgressFunc progress_callback)
   {
      YADOMS_LOG(information) << "Scan for updates...";

      progress_callback(true, 0.0f, i18n::CClientStrings::ScanForUpdates, std::string(), new_CDataContainerSharedPtr());

      // Suspend periodic updates scan
      m_evtHandler.postEvent(kStopNextScanTimerId);

      try
      {
         scan(m_pathProvider);
         progress_callback(true, 100.0f, i18n::CClientStrings::ScanForUpdatesSuccess, std::string(), new_CDataContainerSharedPtr());
      }
      catch (std::exception&)
      {
         progress_callback(false, 100.0f, i18n::CClientStrings::ScanForUpdatesFailed, std::string(), new_CDataContainerSharedPtr());
      }

      // Restart periodic updates scan
      m_evtHandler.postEvent(kStartNextScanTimerId);
   }

   shared::CDataContainerSharedPtr CUpdateManager::getUpdates(bool includePrereleases) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);
      return includePrereleases ? m_allUpdates : m_releasesOnlyUpdates;
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildUpdates(bool includePrereleases,
                                                       const shared::versioning::CSemVer& yadomsLocalVersion,
                                                       shared::CDataContainerSharedPtr yadomsAvailableVersions,
                                                       const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
                                                       shared::CDataContainerSharedPtr pluginsAvailableVersions,
                                                       const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
                                                       shared::CDataContainerSharedPtr widgetsAvailableVersions,
                                                       const std::map<
                                                          std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
                                                       scriptInterpretersLocalVersions,
                                                       shared::CDataContainerSharedPtr scriptInterpretersAvailableVersions) const
   {
      shared::CDataContainerSharedPtr updates = new_CDataContainerSharedPtr();

      const auto yadomsUpdates = buildYadomsList(yadomsLocalVersion,
                                                 yadomsAvailableVersions,
                                                 includePrereleases);
      updates->set("yadoms", yadomsUpdates);

      const auto pluginUpdates = buildPluginList(pluginsLocalVersions,
                                                 pluginsAvailableVersions,
                                                 includePrereleases);
      updates->set("plugins", pluginUpdates);

      const auto widgetUpdates = buildWidgetList(widgetsLocalVersions,
                                                 widgetsAvailableVersions,
                                                 includePrereleases);
      updates->set("widgets", widgetUpdates);

      const auto scriptInterpreterUpdates = buildScriptInterpreterList(scriptInterpretersLocalVersions,
                                                                       scriptInterpretersAvailableVersions,
                                                                       includePrereleases);
      updates->set("scriptInterpreters", scriptInterpreterUpdates);

      return updates;
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildYadomsList(const shared::versioning::CSemVer& localVersion,
                                                          shared::CDataContainerSharedPtr availableVersions,
                                                          bool includePrereleases) const
   {
      // Only updateable items for Yadoms
      return addUpdateableYadoms(localVersion,
                                 availableVersions,
                                 includePrereleases);
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                                   const shared::CDataContainerSharedPtr available_versions,
                                                          bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr list = new_CDataContainerSharedPtr();

      // Add updateable items (ie already installed)
      const auto updateableItems = addUpdateablePlugins(localVersions,
                                                        available_versions,
                                                        includePrereleases);
      list->set("updateable", updateableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewPlugins(localVersions,
                                          available_versions,
                                          includePrereleases);
      list->set("new", newItems);

      return list;
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildWidgetList(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                          shared::CDataContainerSharedPtr available_versions,
                                                          bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr list = new_CDataContainerSharedPtr();

      // Add updateable items (ie already installed)
      const auto updateableItems = addUpdateableWidgets(localVersions,
                                                        available_versions,
                                                        includePrereleases);
      list->set("updateable", updateableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewWidgets(localVersions,
                                          available_versions,
                                          includePrereleases);
      list->set("new", newItems);

      return list;
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildScriptInterpreterList(
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
      const shared::CDataContainerSharedPtr available_versions,
      bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr list = new_CDataContainerSharedPtr();

      // Add updateable items (ie already installed)
      const auto updateableItems = addUpdateableScriptInterpreters(localVersions,
                                                                   available_versions,
                                                                   includePrereleases);
      list->set("updateable", updateableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewScriptInterpreters(localVersions,
                                                     available_versions,
                                                     includePrereleases);
      list->set("new", newItems);

      return list;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addUpdateableYadoms(const shared::versioning::CSemVer& localVersion,
                                                              shared::CDataContainerSharedPtr available_versions,
                                                              bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();

      if (available_versions->exists("changelogUrl"))
         item->set("changelogUrl", available_versions->get<std::string>("changelogUrl"));

      std::map<std::string, shared::CDataContainerSharedPtr> older; // Pass by a map to sort versions list
      std::map<std::string, shared::CDataContainerSharedPtr> newer; // Pass by a map to sort versions list

      for (auto& version : available_versions->get<std::vector<shared::CDataContainerSharedPtr>>("versions"))
      {
         try
         {
            shared::versioning::CSemVer v(version->get<std::string>("version"));

            // Don't add prereleases versions if not asked
            if (!v.getPrerelease().empty() && !includePrereleases)
               continue;

            if (v == localVersion)
               continue;

            shared::CDataContainerSharedPtr versionData;
            versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));
            versionData->set("md5Hash", version->get<std::string>("md5Hash"));

            if (v < localVersion)
               older[version->get<std::string>("version")] = versionData;
            else
               newer[version->get<std::string>("version")] = versionData;
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for Yadoms, will be ignored. " << exception.what();
            version->printToLog(YADOMS_LOG(debug));
         }
      }

      try
      {
         if (!older.empty())
         {
            if (m_developerMode)
            {
               YADOMS_LOG(warning) << "Previous Yadoms versions are available because you are in developer mode : "
                  << "Install a previous version at your own risk, "
                  << "downgraded version may be unable to load your current database";
            }
            else
            {
               older.clear();
            }
         }

         item->set("versions", buildUpdateableVersionsNode(localVersion.toString(),
                                                          older,
                                                          newer));
      }
      catch (std::exception& exception)
      {
         YADOMS_LOG(warning) << "Invalid Yadoms package " << localVersion << ", will be ignored";
         YADOMS_LOG(debug) << "exception : " << exception.what();
      }

      return item;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addUpdateablePlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                                        const shared::CDataContainerSharedPtr available_versions,
                                                               bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr updateableItems = new_CDataContainerSharedPtr();

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updateable modules
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();

            item->set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

            std::map<std::string, shared::CDataContainerSharedPtr> older; // Pass by a map to sort versions list
            std::map<std::string, shared::CDataContainerSharedPtr> newer; // Pass by a map to sort versions list
            if (available_versions->exists(moduleType))
            {
               if (available_versions->exists(moduleType + ".changelogUrl"))
                  item->set("changelogUrl", available_versions->get<std::string>(moduleType + ".changelogUrl"));

               try
               {
                  const auto availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
                  for (auto& version : availableVersionsForItem)
                  {
                     shared::versioning::CSemVer v(version->get<std::string>("version"));

                     // Don't add prereleases versions if not asked
                     if (!v.getPrerelease().empty() && !includePrereleases)
                        continue;

                     if (v == localVersion.second->getVersion())
                        continue;

                     if (!checkDependencies(version))
                        continue;

                     shared::CDataContainerSharedPtr versionData;
                     versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));

                     if (v < localVersion.second->getVersion())
                        older[version->get<std::string>("version")] = versionData;
                     else
                        newer[version->get<std::string>("version")] = versionData;
                  }
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
                  YADOMS_LOG(debug) << "exception : " << exception.what();
               }
            }

            item->set("versions", buildUpdateableVersionsNode(localVersion.second->getVersion().toString(),
                                                             older,
                                                             newer));

            updateableItems->set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return updateableItems;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addNewPlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                                 const shared::CDataContainerSharedPtr available_versions,
                                                        bool includePrereleases)
   {
      shared::CDataContainerSharedPtr newItems = new_CDataContainerSharedPtr();

      for (const auto& moduleType : available_versions->getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         // Pass by a map to sort versions list
         std::map<std::string, shared::CDataContainerSharedPtr> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
            for (auto& version : availableVersionsForItem)
            {
               shared::versioning::CSemVer v(version->get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.getPrerelease().empty() && !includePrereleases)
                  continue;

               if (!checkDependencies(version))
                  continue;

               shared::CDataContainerSharedPtr versionData = new_CDataContainerSharedPtr();
               versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));
               newModuleAvailableVersions[version->get<std::string>("version")] = versionData;
            }
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }


         if (newModuleAvailableVersions.empty())
            continue;

         // Add the module entry to the list
         try
         {
            shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();
            const auto& availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(availableVersionsForItem.begin(),
                                                         availableVersionsForItem.end(),
                                                         [&newestVersionLabel](
                                                         const shared::CDataContainerSharedPtr available_version_for_item)
                                                         {
                                                            return available_version_for_item->get<std::string>(
                                                               "version") == newestVersionLabel;
                                                         });
            item->set("iconUrl", (*newestVersionData)->get<std::string>("iconUrl"));
            if (available_versions->exists(moduleType + ".changelogUrl"))
               item->set("changelogUrl", available_versions->get<std::string>(moduleType + ".changelogUrl"));
            item->set("versions", buildNewVersionsNode(newModuleAvailableVersions));

            newItems->set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return newItems;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addUpdateableWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                               shared::CDataContainerSharedPtr available_versions,
                                                               bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr updateableItems = new_CDataContainerSharedPtr();

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updateable modules
            if (moduleType == "system-deactivated-widget")
               continue;
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();

            // Widget path should be relative to www folder
            const auto widgetPath = shared::tools::CFilesystem::makeRelative(m_pathProvider->webServerPath(),
                                                                             localVersion.second->getPath());

            item->set("iconUrl", (widgetPath / "preview.png").generic_string());

            std::map<std::string, shared::CDataContainerSharedPtr> older; // Pass by a map to sort versions list
            std::map<std::string, shared::CDataContainerSharedPtr> newer; // Pass by a map to sort versions list
            if (available_versions->exists(moduleType))
            {
               if (available_versions->exists(moduleType + ".changelogUrl"))
                  item->set("changelogUrl", available_versions->get<std::string>(moduleType + ".changelogUrl"));

               try
               {
                  const auto availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
                  for (auto& version : availableVersionsForItem)
                  {
                     shared::versioning::CSemVer v(version->get<std::string>("version"));

                     // Don't add prereleases versions if not asked
                     if (!v.getPrerelease().empty() && !includePrereleases)
                        continue;

                     if (v == localVersion.second->getVersion())
                        continue;

                     if (!checkDependencies(version))
                        continue;

                     shared::CDataContainerSharedPtr versionData = new_CDataContainerSharedPtr();
                     versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));

                     if (v < localVersion.second->getVersion())
                        older[version->get<std::string>("version")] = versionData;
                     else
                        newer[version->get<std::string>("version")] = versionData;
                  }
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
                  YADOMS_LOG(debug) << "exception : " << exception.what();
               }
            }

            item->set("versions", buildUpdateableVersionsNode(localVersion.second->getVersion().toString(),
                                                             older,
                                                             newer));

            updateableItems->set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return updateableItems;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addNewWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                                 const shared::CDataContainerSharedPtr available_versions,
                                                        bool includePrereleases)
   {
      shared::CDataContainerSharedPtr newItems = new_CDataContainerSharedPtr();

      for (const auto& moduleType : available_versions->getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         // Pass by a map to sort versions list
         std::map<std::string, shared::CDataContainerSharedPtr> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForModule = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
            for (auto& version : availableVersionsForModule)
            {
               shared::versioning::CSemVer v(version->get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.getPrerelease().empty() && !includePrereleases)
                  continue;

               if (!checkDependencies(version))
                  continue;

               shared::CDataContainerSharedPtr versionData = new_CDataContainerSharedPtr();
               versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));
               newModuleAvailableVersions[version->get<std::string>("version")] = versionData;
            }
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }


         if (newModuleAvailableVersions.empty())
            continue;

         // Add the module entry to the list
         try
         {
            shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();
            const auto& availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(availableVersionsForItem.begin(),
                                                         availableVersionsForItem.end(),
                                                         [&newestVersionLabel](
                                                         shared::CDataContainerSharedPtr available_version_for_item)
                                                         {
                                                            return available_version_for_item->get<std::string>(
                                                               "version") == newestVersionLabel;
                                                         });
            item->set("iconUrl", (*newestVersionData)->get<std::string>("previewUrl"));
            if (available_versions->exists(moduleType + ".changelogUrl"))
               item->set("changelogUrl", available_versions->get<std::string>(moduleType + ".changelogUrl"));
            item->set("versions", buildNewVersionsNode(newModuleAvailableVersions));

            newItems->set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return newItems;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addUpdateableScriptInterpreters(
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
      shared::CDataContainerSharedPtr available_versions,
      bool includePrereleases) const
   {
      shared::CDataContainerSharedPtr updateableItems = new_CDataContainerSharedPtr();

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updateable modules
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();

            item->set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

            std::map<std::string, shared::CDataContainerSharedPtr> older; // Pass by a map to sort versions list
            std::map<std::string, shared::CDataContainerSharedPtr> newer; // Pass by a map to sort versions list
            if (available_versions->exists(moduleType))
            {
               if (available_versions->exists(moduleType + ".changelogUrl"))
                  item->set("changelogUrl", available_versions->get<std::string>(moduleType + ".changelogUrl"));

               try
               {
                  const auto availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
                  for (auto& version : availableVersionsForItem)
                  {
                     shared::versioning::CSemVer v(version->get<std::string>("version"));

                     // Don't add prereleases versions if not asked
                     if (!v.getPrerelease().empty() && !includePrereleases)
                        continue;

                     if (v == localVersion.second->getVersion())
                        continue;

                     if (!checkDependencies(version))
                        continue;

                     shared::CDataContainerSharedPtr versionData = new_CDataContainerSharedPtr();
                     versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));

                     if (v < localVersion.second->getVersion())
                        older[version->get<std::string>("version")] = versionData;
                     else
                        newer[version->get<std::string>("version")] = versionData;
                  }
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
                  YADOMS_LOG(debug) << "exception : " << exception.what();
               }
            }

            item->set("versions", buildUpdateableVersionsNode(localVersion.second->getVersion().toString(),
                                                             older,
                                                             newer));

            updateableItems->set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return updateableItems;
   }

   shared::CDataContainerSharedPtr CUpdateManager::addNewScriptInterpreters(
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
      shared::CDataContainerSharedPtr available_versions,
      bool includePrereleases)
   {
      shared::CDataContainerSharedPtr newItems = new_CDataContainerSharedPtr();

      for (const auto& moduleType : available_versions->getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         // Pass by a map to sort versions list
         std::map<std::string, shared::CDataContainerSharedPtr> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForModule = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
            for (auto& version : availableVersionsForModule)
            {
               shared::versioning::CSemVer v(version->get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.getPrerelease().empty() && !includePrereleases)
                  continue;

               if (!checkDependencies(version))
                  continue;

               shared::CDataContainerSharedPtr versionData = new_CDataContainerSharedPtr();
               versionData->set("downloadUrl", version->get<std::string>("downloadUrl"));
               newModuleAvailableVersions[version->get<std::string>("version")] = versionData;
            }
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }


         if (newModuleAvailableVersions.empty())
            continue;

         // Add the module entry to the list
         try
         {
            shared::CDataContainerSharedPtr item = new_CDataContainerSharedPtr();
            const auto& availableVersionsForItem = available_versions->get<std::vector<shared::CDataContainerSharedPtr>>(moduleType + ".versions");
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(availableVersionsForItem.begin(),
                                                         availableVersionsForItem.end(),
                                                         [&newestVersionLabel](
                                                         shared::CDataContainerSharedPtr available_version_for_item)
                                                         {
                                                            return available_version_for_item->get<std::string>(
                                                               "version") == newestVersionLabel;
                                                         });
            item->set("iconUrl", (*newestVersionData)->get<std::string>("iconUrl"));
            if (available_versions->exists(moduleType + ".changelogUrl"))
               item->set("changelogUrl", available_versions->get<std::string>(moduleType + ".changelogUrl"));
            item->set("versions", buildNewVersionsNode(newModuleAvailableVersions));

            newItems->set(moduleType, item);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "Invalid remote package for " << moduleType << ", will be ignored";
            YADOMS_LOG(debug) << "exception : " << exception.what();
         }
      }

      return newItems;
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildUpdateableVersionsNode(const std::string& installed,
                                                                      std::map<std::string, shared::CDataContainerSharedPtr> older,
                                                                      std::map<std::string, shared::CDataContainerSharedPtr> newer)
   {
      shared::CDataContainerSharedPtr versions = new_CDataContainerSharedPtr();
      versions->set("installed", installed);

      if (!older.empty())
      {
         // Sort (newer version first)
         shared::CDataContainerSharedPtr olderVersions = new_CDataContainerSharedPtr();
         for (auto v = older.rbegin(); v != older.rend(); ++v)
            // Force different path char to not cut version string into subPaths
            olderVersions->set(v->first, v->second, 0);
         versions->set("older", olderVersions);
      }

      if (!newer.empty())
      {
         // Sort (newer version first)
         shared::CDataContainerSharedPtr newerVersions = new_CDataContainerSharedPtr();
         for (auto v = newer.rbegin(); v != newer.rend(); ++v)
            // Force different path char to not cut version string into subPaths
            newerVersions->set(v->first, v->second, 0);
         versions->set("newer", newerVersions);

         // Find the newest version
         shared::CDataContainerSharedPtr newestVersion = new_CDataContainerSharedPtr();
         // Force different path char to not cut version string into subPaths
         newestVersion->set(newer.rbegin()->first, newer.rbegin()->second, 0);
         versions->set("newest", newestVersion);
      }

      return versions;
   }

   shared::CDataContainerSharedPtr CUpdateManager::buildNewVersionsNode(const std::map<std::string, shared::CDataContainerSharedPtr>& newItemAvailableVersions)
   {
      shared::CDataContainerSharedPtr versions = new_CDataContainerSharedPtr();

      // Sort (newer version first)
      shared::CDataContainerSharedPtr sortedVersions = new_CDataContainerSharedPtr();
      for (auto v = newItemAvailableVersions.rbegin(); v != newItemAvailableVersions.rend(); ++v)
         // Force different path char to not cut version string into subPaths
         sortedVersions->set(v->first, v->second, 0);
      versions->set("versions", sortedVersions);

      // Find the newest version
      shared::CDataContainerSharedPtr newestVersion = new_CDataContainerSharedPtr();
      // Force different path char to not cut version string into subPaths
      newestVersion->set(newItemAvailableVersions.rbegin()->first,
                        newItemAvailableVersions.rbegin()->second, 0);
      versions->set("newest", newestVersion);

      return versions;
   }

   void CUpdateManager::notifyNewUpdateAvailable() const
   {
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kUpdateAvailable, "yadoms", std::string());
   }

   bool CUpdateManager::startTask(const boost::shared_ptr<task::ITask> task,
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
      std::string taskId;
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
                                                             boost::bind(&worker::CPlugin::update,
                                                                         _1,
                                                                         std::string(pluginName),
                                                                         std::string(downloadUrl),
                                                                         m_pluginManager,
                                                                         boost::filesystem::path(m_pathProvider->pluginsPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::installPluginAsync(const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("plugin.install",
                                                             boost::bind(&worker::CPlugin::install,
                                                                         _1,
                                                                         std::string(downloadUrl),
                                                                         m_pluginManager,
                                                                         boost::filesystem::path(m_pathProvider->pluginsPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::removePluginAsync(const std::string& pluginName) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("plugin.remove",
                                                             boost::bind(&worker::CPlugin::remove,
                                                                         _1,
                                                                         std::string(pluginName),
                                                                         m_pluginManager,
                                                                         boost::filesystem::path(m_pathProvider->pluginsPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::updateWidgetAsync(const std::string& widgetName,
                                                 const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("widget.update",
                                                             boost::bind(&worker::CWidget::update,
                                                                         _1,
                                                                         std::string(widgetName),
                                                                         std::string(downloadUrl),
                                                                         boost::filesystem::path(m_pathProvider->widgetsPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::installWidgetAsync(const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("widget.install",
                                                             boost::bind(&worker::CWidget::install,
                                                                         _1,
                                                                         std::string(downloadUrl),
                                                                         boost::filesystem::path(m_pathProvider->widgetsPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::removeWidgetAsync(const std::string& widgetName) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("widget.remove",
                                                             boost::bind(&worker::CWidget::remove,
                                                                         _1,
                                                                         std::string(widgetName),
                                                                         boost::filesystem::path(m_pathProvider->widgetsPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }


   std::string CUpdateManager::updateScriptInterpreterAsync(const std::string& scriptInterpreterName,
                                                            const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("scriptInterpreter.update",
                                                             boost::bind(&worker::CScriptInterpreter::update,
                                                                         _1,
                                                                         std::string(scriptInterpreterName),
                                                                         std::string(downloadUrl),
                                                                         boost::filesystem::path(m_pathProvider->scriptInterpretersPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::installScriptInterpreterAsync(const std::string& downloadUrl) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("scriptInterpreter.install",
                                                             boost::bind(&worker::CScriptInterpreter::install,
                                                                         _1,
                                                                         std::string(downloadUrl),
                                                                         boost::filesystem::path(m_pathProvider->scriptInterpretersPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const
   {
      const auto task(boost::make_shared<task::CGenericTask>("scriptInterpreter.remove",
                                                             boost::bind(&worker::CScriptInterpreter::install,
                                                                         _1,
                                                                         std::string(scriptInterpreterName),
                                                                         boost::filesystem::path(m_pathProvider->scriptInterpretersPath()))));
      //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   std::string CUpdateManager::findMd5HashAssociatedTo(const std::string& downloadUrl,
                                                       const std::string& allUpdatesNode) const
   {
      if (m_allUpdates->exists(allUpdatesNode, '|'))
      {
         auto versions = m_allUpdates->getAsMap<shared::CDataContainerSharedPtr>(allUpdatesNode, '|'); // Don't use default pathChar('.') because also contained in version name (like '2.3.0')
         const auto versionInfo = std::find_if(versions.begin(),
                                               versions.end(),
                                               [&downloadUrl](const auto& version)
                                               {
                                                  return version.second->get<std::string>("downloadUrl") == downloadUrl;
                                               });

         if (versionInfo == versions.end())
            return std::string();

         return versionInfo->second->get<std::string>("md5Hash");
      }
      return std::string();
   }

   std::string CUpdateManager::updateYadomsAsync(const std::string& downloadUrl) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);

      // Find expected MD5Hash corresponding to the download URL
      auto expectedMd5Hash = findMd5HashAssociatedTo(downloadUrl, "yadoms|versions|newer");
      if (expectedMd5Hash.empty())
         expectedMd5Hash = findMd5HashAssociatedTo(downloadUrl, "yadoms|versions|older");
      if (expectedMd5Hash.empty())
         throw std::runtime_error("Version not found");

      const auto task(boost::make_shared<task::CGenericTask>("yadoms.update",
                                                             boost::bind(&worker::CYadoms::update, _1,
                                                                         std::string(downloadUrl),
                                                                         expectedMd5Hash)));
      //force to copy parameter because the versionToInstall is a reference and cannot be used "as is" in async task
      return startTask(task);
   }

   bool CUpdateManager::checkDependencies(const shared::CDataContainerSharedPtr item_version_node)
   {
      if (!item_version_node->containsChild("dependencies"))
         return true;

      // For now, only Yadoms minimum version check is supported
      const shared::versioning::CSemVer minSupportedYadomsVersion(item_version_node->get<std::string>("dependencies.yadoms.minimumVersion"));
      const auto& actualYadomsVersion = shared::CServiceLocator::instance().get<IRunningInformation>()->getSoftwareVersion().getVersion();

      return actualYadomsVersion >= minSupportedYadomsVersion;
   }
} // namespace update
