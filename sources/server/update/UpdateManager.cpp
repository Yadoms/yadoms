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
#include <shared/ServiceLocator.h>
#include "database/entities/Entities.h"

#include <utility>


namespace update
{
   static constexpr auto NextScanTimerEventId = shared::event::kUserFirstId;

   CUpdateManager::CUpdateManager(boost::shared_ptr<task::CScheduler> taskScheduler,
                                  boost::shared_ptr<pluginSystem::CManager> pluginManager,
                                  boost::shared_ptr<automation::interpreter::IManager> interpreterManager,
                                  boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                  bool developerMode,
                                  boost::shared_ptr<const IPathProvider> pathProvider)
      : m_taskScheduler(std::move(taskScheduler)),
        m_pluginManager(std::move(pluginManager)),
        m_interpreterManager(std::move(interpreterManager)),
        m_eventLogger(std::move(eventLogger)),
        m_developerMode(developerMode),
        m_pathProvider(std::move(pathProvider)),
        m_thread(boost::thread(&CUpdateManager::doWork, this))
   {
   }

   CUpdateManager::~CUpdateManager()
   {
      m_thread.interrupt();
      m_thread.timed_join(boost::posix_time::seconds(30));
   }

   void CUpdateManager::doWork()
   {
      YADOMS_LOG_CONFIGURE("CUpdateManager")
      YADOMS_LOG(debug) << "Start";
      try
      {
         static const boost::posix_time::time_duration FirstScanDelay = boost::posix_time::seconds(20);
         static const boost::posix_time::time_duration ScanPeriod = boost::posix_time::hours(24);

         const auto nexScanTimer = m_evtHandler.createTimer(NextScanTimerEventId,
                                                            shared::event::CEventTimer::kOneShot,
                                                            FirstScanDelay);

         while (true)
         {
            switch (m_evtHandler.waitForEvents())
            {
            case NextScanTimerEventId:
               {
                  YADOMS_LOG(information) << "Scan for available updates...";
                  scanForUpdates();
                  nexScanTimer->start(ScanPeriod);
                  break;
               }

            default:
               {
                  YADOMS_LOG(error) << "Unsupported event " << m_evtHandler.getEventId() << ", ignored";
                  break;
               }
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(debug) << "Interruption";
      }
   }

   void CUpdateManager::getAvailableVersionsFromServer()
   {
      try
      {
         YADOMS_LOG(debug) << "Get available updates...";

         const auto yadomsAvailableVersions = info::CUpdateSite::getAllYadomsVersions();

         YADOMS_LOG(debug) << "Read Plugins versions...";
         const auto pluginsAvailableVersions = info::CUpdateSite::getAllPluginVersions();

         YADOMS_LOG(debug) << "Read Widgets versions...";
         const auto widgetsAvailableVersions = info::CUpdateSite::getAllWidgetVersions();

         YADOMS_LOG(debug) << "Read ScriptInterpreters versions...";
         const auto scriptInterpretersAvailableVersions = info::CUpdateSite::getAllScriptInterpreterVersions();

         YADOMS_LOG(debug) << "Get available updates OK";

         boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);
         m_yadomsAvailableVersions = yadomsAvailableVersions;
         m_pluginsAvailableVersions = pluginsAvailableVersions;
         m_widgetsAvailableVersions = widgetsAvailableVersions;
         m_scriptInterpretersAvailableVersions = scriptInterpretersAvailableVersions;
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << "Error getting available updates (do you have a working Internet connection ?), " << e.what();
         throw;
      }
   }

   bool CUpdateManager::updateAvailable(const boost::shared_ptr<shared::CDataContainer>& updates,
                                        const std::string& componentTag)
   {
      const auto nodePath = componentTag + ".updateable";
      if (!updates->exists(nodePath))
         return false;

      for (const auto& component : updates->getKeys(nodePath))
      {
         auto versionPath = nodePath;
         auto key = versionPath.append(".").append(component).append(".versions.newer");
         if (updates->exists(key) && !updates->getKeys(key).empty())
            return true;
      }

      return false;
   }

   bool CUpdateManager::updateAvailable(bool includePrereleases) const
   {
      const auto updates = getUpdates(includePrereleases);

      if (!updates->getKeys("yadoms.versions.newer").empty())
         return true;

      if (updateAvailable(updates, "plugins")
         || updateAvailable(updates, "widgets")
         || updateAvailable(updates, "scriptInterpreters"))
         return true;

      return false;
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::getUpdates(bool includePrereleases) const
   {
      if (!m_yadomsAvailableVersions || m_yadomsAvailableVersions->empty()
         || !m_pluginsAvailableVersions || m_pluginsAvailableVersions->empty()
         || !m_widgetsAvailableVersions || m_widgetsAvailableVersions->empty()
         || !m_scriptInterpretersAvailableVersions || m_scriptInterpretersAvailableVersions->empty())
      {
         YADOMS_LOG(debug) << "Available versions was not provided";
         return shared::CDataContainer::make();
      }

      // Read inputs
      const auto yadomsLocalVersion = shared::CServiceLocator::instance()
                                      .get<IRunningInformation>()->getSoftwareVersion().getVersion();
      const auto pluginsLocalVersions = m_pluginManager->getPluginList();
      const auto widgetsLocalVersions = worker::CWidget::getWidgetList(m_pathProvider->widgetsPath());
      const auto scriptInterpretersLocalVersions = m_interpreterManager->getLoadedInterpretersInformation();

      return buildUpdates(includePrereleases,
                          yadomsLocalVersion,
                          m_yadomsAvailableVersions,
                          pluginsLocalVersions,
                          m_pluginsAvailableVersions,
                          widgetsLocalVersions,
                          m_widgetsAvailableVersions,
                          scriptInterpretersLocalVersions,
                          m_scriptInterpretersAvailableVersions);
   }

   void CUpdateManager::asyncScanForUpdates()
   {
      m_evtHandler.postEvent(NextScanTimerEventId);
   }

   void CUpdateManager::scanForUpdates()
   {
      YADOMS_LOG(information) << "Scan for updates...";

      try
      {
         getAvailableVersionsFromServer();

         // Notify only for new releases (not for prereleases)
         if (updateAvailable(false))
            notifyNewUpdateAvailable();

         YADOMS_LOG(information) << "Scan for updates success";
      }
      catch (std::exception&)
      {
         YADOMS_LOG(warning) << "Scan for updates failed";
      }
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildUpdates(
      bool includePrereleases,
      const shared::versioning::CSemVer& yadomsLocalVersion,
      const boost::shared_ptr<shared::CDataContainer>& yadomsAvailableVersions,
      const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
      const boost::shared_ptr<shared::CDataContainer>& pluginsAvailableVersions,
      const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
      const boost::shared_ptr<shared::CDataContainer>& widgetsAvailableVersions,
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& scriptInterpretersLocalVersions,
      const boost::shared_ptr<shared::CDataContainer>& scriptInterpretersAvailableVersions) const
   {
      auto updates = shared::CDataContainer::make();

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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildYadomsList(
      const shared::versioning::CSemVer& localVersion,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      // Only updateable items for Yadoms
      return addUpdateableYadoms(localVersion,
                                 availableVersions,
                                 includePrereleases);
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildPluginList(
      const pluginSystem::IFactory::AvailablePluginMap& localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      auto list = shared::CDataContainer::make();

      // Add updateable items (ie already installed)
      const auto updateableItems = addUpdateablePlugins(localVersions,
                                                        availableVersions,
                                                        includePrereleases);
      list->set("updateable", updateableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewPlugins(localVersions,
                                          availableVersions,
                                          includePrereleases);
      list->set("new", newItems);

      return list;
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildWidgetList(
      const worker::CWidget::AvailableWidgetMap& localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      boost::shared_ptr<shared::CDataContainer> list = shared::CDataContainer::make();

      // Add updateable items (ie already installed)
      const auto updateableItems = addUpdateableWidgets(localVersions,
                                                        availableVersions,
                                                        includePrereleases);
      list->set("updateable", updateableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewWidgets(localVersions,
                                          availableVersions,
                                          includePrereleases);
      list->set("new", newItems);

      return list;
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildScriptInterpreterList(
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
      localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      boost::shared_ptr<shared::CDataContainer> list = shared::CDataContainer::make();

      // Add updateable items (ie already installed)
      const auto updateableItems = addUpdateableScriptInterpreters(localVersions,
                                                                   availableVersions,
                                                                   includePrereleases);
      list->set("updateable", updateableItems);

      // Add items not already installed (ie not in localVersions list)
      const auto newItems = addNewScriptInterpreters(localVersions,
                                                     availableVersions,
                                                     includePrereleases);
      list->set("new", newItems);

      return list;
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addUpdateableYadoms(
      const shared::versioning::CSemVer& localVersion,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      boost::shared_ptr<shared::CDataContainer> item = shared::CDataContainer::make();

      if (availableVersions->exists("changelogUrl"))
         item->set("changelogUrl", availableVersions->get<std::string>("changelogUrl"));

      std::map<std::string, boost::shared_ptr<shared::CDataContainer>> older; // Pass by a map to sort versions list
      std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newer; // Pass by a map to sort versions list

      for (const auto& version : availableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("versions"))
      {
         try
         {
            shared::versioning::CSemVer v(version->get<std::string>("version"));

            // Don't add prereleases versions if not asked
            if (!v.getPrerelease().empty() && !includePrereleases)
               continue;

            if (v == localVersion)
               continue;

            const auto versionData = shared::CDataContainer::make();
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addUpdateablePlugins(
      const pluginSystem::IFactory::AvailablePluginMap& localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      auto updateableItems = shared::CDataContainer::make();

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updateable modules
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            auto item = shared::CDataContainer::make();

            item->set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

            // Use maps to sort versions list
            std::map<std::string, boost::shared_ptr<shared::CDataContainer>> older;
            std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newer;

            if (availableVersions->exists(moduleType))
            {
               if (availableVersions->exists(moduleType + ".changelogUrl"))
                  item->set("changelogUrl", availableVersions->get<std::string>(moduleType + ".changelogUrl"));

               if (availableVersions->getWithDefault<bool>(moduleType + ".obsolete", false))
               {
                  item->set("obsolete", true);
               }
               else
               {
                  try
                  {
                     const auto availableVersionsForItem = availableVersions->get<std::vector<boost::shared_ptr<shared::
                        CDataContainer>>>(moduleType + ".versions");
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

                        const auto versionData = shared::CDataContainer::make();
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addNewPlugins(
      const pluginSystem::IFactory::AvailablePluginMap& localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases)
   {
      auto newItems = shared::CDataContainer::make();

      for (const auto& moduleType : availableVersions->getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         if (availableVersions->getWithDefault<bool>(moduleType + ".obsolete", false))
            continue; // Don't propose obsolete modules

         // Pass by a map to sort versions list
         std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForItem = availableVersions->get<std::vector<boost::shared_ptr<shared::
               CDataContainer>>>(moduleType + ".versions");
            for (auto& version : availableVersionsForItem)
            {
               shared::versioning::CSemVer v(version->get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.getPrerelease().empty() && !includePrereleases)
                  continue;

               if (!checkDependencies(version))
                  continue;

               const auto versionData = shared::CDataContainer::make();
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
            auto item = shared::CDataContainer::make();
            const auto& availableVersionsForItem =
               availableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>(moduleType + ".versions");
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(
               availableVersionsForItem.begin(),
               availableVersionsForItem.end(),
               [&newestVersionLabel](const boost::shared_ptr<shared::CDataContainer>& availableVersionForItem)
               {
                  return availableVersionForItem->get<std::string>("version") == newestVersionLabel;
               });
            item->set("iconUrl", (*newestVersionData)->get<std::string>("iconUrl"));
            if (availableVersions->exists(moduleType + ".changelogUrl"))
               item->set("changelogUrl", availableVersions->get<std::string>(moduleType + ".changelogUrl"));
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addUpdateableWidgets(
      const worker::CWidget::AvailableWidgetMap& localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      auto updateableItems = shared::CDataContainer::make();

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

            auto item = shared::CDataContainer::make();

            // Widget path should be relative to www folder
            const auto widgetPath = shared::tools::CFilesystem::makeRelative(m_pathProvider->webServerPath(),
                                                                             localVersion.second->getPath());

            item->set("iconUrl", (widgetPath / "preview.png").generic_string());

            // Use maps to sort versions list
            std::map<std::string, boost::shared_ptr<shared::CDataContainer>> older;
            std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newer;

            if (availableVersions->exists(moduleType))
            {
               if (availableVersions->exists(moduleType + ".changelogUrl"))
                  item->set("changelogUrl", availableVersions->get<std::string>(moduleType + ".changelogUrl"));

               if (availableVersions->getWithDefault<bool>(moduleType + ".obsolete", false))
               {
                  item->set("obsolete", true);
               }
               else
               {
                  try
                  {
                     const auto availableVersionsForItem = availableVersions->get<std::vector<boost::shared_ptr<shared::
                        CDataContainer>>>(moduleType + ".versions");
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

                        const auto versionData = shared::CDataContainer::make();
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addNewWidgets(
      const worker::CWidget::AvailableWidgetMap& localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases)
   {
      auto newItems = shared::CDataContainer::make();

      for (const auto& moduleType : availableVersions->getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed         

         if (availableVersions->getWithDefault<bool>(moduleType + ".obsolete", false))
            continue; // Don't propose obsolete modules

         // Pass by a map to sort versions list
         std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForModule =
               availableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>(moduleType + ".versions");
            for (auto& version : availableVersionsForModule)
            {
               shared::versioning::CSemVer v(version->get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.getPrerelease().empty() && !includePrereleases)
                  continue;

               if (!checkDependencies(version))
                  continue;

               const auto versionData = shared::CDataContainer::make();
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
            auto item = shared::CDataContainer::make();
            const auto& availableVersionsForItem = availableVersions->get<std::vector<boost::shared_ptr<shared::
               CDataContainer>>>(moduleType + ".versions");
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(
               availableVersionsForItem.begin(),
               availableVersionsForItem.end(),
               [&newestVersionLabel](const boost::shared_ptr<shared::CDataContainer>& availableVersionForItem)
               {
                  return availableVersionForItem->get<std::string>("version") == newestVersionLabel;
               });
            item->set("iconUrl", (*newestVersionData)->get<std::string>("previewUrl"));
            if (availableVersions->exists(moduleType + ".changelogUrl"))
               item->set("changelogUrl", availableVersions->get<std::string>(moduleType + ".changelogUrl"));
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addUpdateableScriptInterpreters(
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
      localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases) const
   {
      auto updateableItems = shared::CDataContainer::make();

      for (const auto& localVersion : localVersions)
      {
         try
         {
            const auto moduleType = localVersion.first;

            // Filter non-updateable modules
            if (m_developerMode && boost::starts_with(moduleType, "dev-"))
               continue;

            auto item = shared::CDataContainer::make();

            item->set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

            std::map<std::string, boost::shared_ptr<shared::CDataContainer>> older;
            // Pass by a map to sort versions list
            std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newer;
            // Pass by a map to sort versions list
            if (availableVersions->exists(moduleType))
            {
               if (availableVersions->exists(moduleType + ".changelogUrl"))
                  item->set("changelogUrl", availableVersions->get<std::string>(moduleType + ".changelogUrl"));

               if (availableVersions->getWithDefault<bool>(moduleType + ".obsolete", false))
               {
                  item->set("obsolete", true);
               }
               else
               {
                  try
                  {
                     const auto availableVersionsForItem =
                        availableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>(moduleType + ".versions");
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

                        const auto versionData = shared::CDataContainer::make();
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::addNewScriptInterpreters(
      const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
      localVersions,
      const boost::shared_ptr<shared::CDataContainer>& availableVersions,
      bool includePrereleases)
   {
      auto newItems = shared::CDataContainer::make();

      for (const auto& moduleType : availableVersions->getKeys())
      {
         if (localVersions.find(moduleType) != localVersions.end())
            continue; // Module already installed

         if (availableVersions->getWithDefault<bool>(moduleType + ".obsolete", false))
            continue; // Don't propose obsolete modules
         //
         // Pass by a map to sort versions list
         std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newModuleAvailableVersions;
         try
         {
            const auto availableVersionsForModule =
               availableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>(moduleType + ".versions");
            for (auto& version : availableVersionsForModule)
            {
               shared::versioning::CSemVer v(version->get<std::string>("version"));

               // Don't add prereleases versions if not asked
               if (!v.getPrerelease().empty() && !includePrereleases)
                  continue;

               if (!checkDependencies(version))
                  continue;

               const auto versionData = shared::CDataContainer::make();
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
            boost::shared_ptr<shared::CDataContainer> item = shared::CDataContainer::make();
            const auto& availableVersionsForItem =
               availableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>(moduleType + ".versions");
            const auto& newestVersionLabel = newModuleAvailableVersions.rbegin()->first;
            const auto& newestVersionData = std::find_if(
               availableVersionsForItem.begin(),
               availableVersionsForItem.end(),
               [&newestVersionLabel](const boost::shared_ptr<shared::CDataContainer>& availableVersionForItem)
               {
                  return availableVersionForItem->get<std::string>("version") == newestVersionLabel;
               });
            item->set("iconUrl", (*newestVersionData)->get<std::string>("iconUrl"));
            if (availableVersions->exists(moduleType + ".changelogUrl"))
               item->set("changelogUrl", availableVersions->get<std::string>(moduleType + ".changelogUrl"));
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

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildUpdateableVersionsNode(
      const std::string& installed,
      std::map<std::string, boost::shared_ptr<shared::CDataContainer>> older,
      std::map<std::string, boost::shared_ptr<shared::CDataContainer>> newer)
   {
      auto versions = shared::CDataContainer::make();
      versions->set("installed", installed);

      if (!older.empty())
      {
         // Sort (newer version first)
         const auto olderVersions = shared::CDataContainer::make();
         for (auto v = older.rbegin(); v != older.rend(); ++v)
            // Force different path char to not cut version string into subPaths
            olderVersions->set(v->first, v->second, 0);
         versions->set("older", olderVersions);
      }

      if (!newer.empty())
      {
         // Sort (newer version first)
         const auto newerVersions = shared::CDataContainer::make();
         for (auto v = newer.rbegin(); v != newer.rend(); ++v)
            // Force different path char to not cut version string into subPaths
            newerVersions->set(v->first, v->second, 0);
         versions->set("newer", newerVersions);

         // Find the newest version
         const auto newestVersion = shared::CDataContainer::make();
         // Force different path char to not cut version string into subPaths
         newestVersion->set(newer.rbegin()->first, newer.rbegin()->second, 0);
         versions->set("newest", newestVersion);
      }

      return versions;
   }

   boost::shared_ptr<shared::CDataContainer> CUpdateManager::buildNewVersionsNode(
      const std::map<std::string,
                     boost::shared_ptr<shared::CDataContainer>>& newItemAvailableVersions)
   {
      auto versions = shared::CDataContainer::make();

      // Sort (newer version first)
      const auto sortedVersions = shared::CDataContainer::make();
      for (auto v = newItemAvailableVersions.rbegin(); v != newItemAvailableVersions.rend(); ++v)
         // Force different path char to not cut version string into subPaths
         sortedVersions->set(v->first, v->second, 0);
      versions->set("versions", sortedVersions);

      // Find the newest version
      const auto newestVersion = shared::CDataContainer::make();
      // Force different path char to not cut version string into subPaths
      newestVersion->set(newItemAvailableVersions.rbegin()->first,
                         newItemAvailableVersions.rbegin()->second, 0);
      versions->set("newest", newestVersion);

      return versions;
   }

   void CUpdateManager::notifyNewUpdateAvailable() const
   {
      YADOMS_LOG(information) << "Some updates are available";
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kUpdateAvailable, "yadoms", std::string());
   }

   bool CUpdateManager::startTask(const boost::shared_ptr<task::ITask>& task,
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

   std::string CUpdateManager::startTask(const boost::shared_ptr<task::ITask>& task) const
   {
      std::string taskId;
      if (startTask(task, taskId))
         return taskId;
      throw shared::exception::CException("Fail to start task");
   }

   std::string CUpdateManager::updatePluginAsync(const std::string& pluginName,
                                                 const std::string& downloadUrl) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "plugin.update",
         [pluginName, downloadUrl, this](auto taskProgressFunc)
         {
            return worker::CPlugin::update(taskProgressFunc,
                                           pluginName,
                                           downloadUrl,
                                           m_pluginManager,
                                           m_pathProvider->pluginsPath());
         }));
      return startTask(task);
   }

   std::string CUpdateManager::installPluginAsync(const std::string& downloadUrl) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "plugin.install",
         [downloadUrl, this](auto taskProgressFunc)
         {
            return worker::CPlugin::install(taskProgressFunc,
                                            downloadUrl,
                                            m_pluginManager,
                                            m_pathProvider->pluginsPath());
         }));
      return startTask(task);
   }

   std::string CUpdateManager::removePluginAsync(const std::string& pluginName) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "plugin.remove",
         [pluginName, this](auto taskProgressFunc)
         {
            return worker::CPlugin::remove(taskProgressFunc,
                                           pluginName,
                                           m_pluginManager,
                                           m_pathProvider->pluginsPath());
         }));
      return startTask(task);
   }

   std::string CUpdateManager::updateWidgetAsync(const std::string& widgetName,
                                                 const std::string& downloadUrl) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "widget.update",
         [widgetName, downloadUrl, widgetsPath = m_pathProvider->widgetsPath()](auto taskProgressFunc)
         {
            return worker::CWidget::update(taskProgressFunc,
                                           widgetName,
                                           downloadUrl,
                                           widgetsPath);
         }));
      return startTask(task);
   }

   std::string CUpdateManager::installWidgetAsync(const std::string& downloadUrl) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "widget.install",
         [downloadUrl, widgetsPath = m_pathProvider->widgetsPath()](auto taskProgressFunc)
         {
            return worker::CWidget::install(taskProgressFunc,
                                            downloadUrl,
                                            widgetsPath);
         }));
      return startTask(task);
   }

   std::string CUpdateManager::removeWidgetAsync(const std::string& widgetName) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "widget.remove",
         [widgetName, widgetsPath = m_pathProvider->widgetsPath()](auto taskProgressFunc)
         {
            return worker::CWidget::remove(taskProgressFunc,
                                           widgetName,
                                           widgetsPath);
         }));
      return startTask(task);
   }


   std::string CUpdateManager::updateScriptInterpreterAsync(const std::string& scriptInterpreterName,
                                                            const std::string& downloadUrl) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "scriptInterpreter.update",
         [scriptInterpreterName, downloadUrl, scriptInterpretersPath = m_pathProvider->scriptInterpretersPath()](auto taskProgressFunc)
         {
            return worker::CScriptInterpreter::update(taskProgressFunc,
                                                      scriptInterpreterName,
                                                      downloadUrl,
                                                      scriptInterpretersPath);
         }));
      return startTask(task);
   }

   std::string CUpdateManager::installScriptInterpreterAsync(const std::string& downloadUrl) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "scriptInterpreter.install",
         [downloadUrl, scriptInterpretersPath = m_pathProvider->scriptInterpretersPath()](auto taskProgressFunc)
         {
            return worker::CScriptInterpreter::install(taskProgressFunc,
                                                       downloadUrl,
                                                       scriptInterpretersPath);
         }));
      return startTask(task);
   }

   std::string CUpdateManager::removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const
   {
      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>(
         "scriptInterpreter.remove",
         [scriptInterpreterName, scriptInterpretersPath = m_pathProvider->scriptInterpretersPath()](auto taskProgressFunc)
         {
            worker::CScriptInterpreter::remove(taskProgressFunc,
                                               scriptInterpreterName,
                                               scriptInterpretersPath);
         }));
      return startTask(task);
   }

   std::string CUpdateManager::findMd5HashAssociatedTo(const std::string& downloadUrl) const
   {
      if (!m_yadomsAvailableVersions->exists("versions")
         || !m_yadomsAvailableVersions->isArray("versions"))
         return std::string();

      const auto versions = m_yadomsAvailableVersions->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("versions");
      const auto versionInfo = std::find_if(versions.begin(),
                                            versions.end(),
                                            [&downloadUrl](const auto& version)
                                            {
                                               return version->get("downloadUrl") == downloadUrl;
                                            });

      if (versionInfo == versions.end())
         return std::string();

      return (*versionInfo)->get<std::string>("md5Hash");
   }

   std::string CUpdateManager::updateYadomsAsync(const std::string& downloadUrl) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_updateMutex);

      // Find expected MD5Hash corresponding to the download URL
      auto expectedMd5Hash = findMd5HashAssociatedTo(downloadUrl);
      if (expectedMd5Hash.empty())
         throw std::runtime_error("Version not found");

      //force to copy parameters because references cannot be used in async task
      const auto task(boost::make_shared<task::CGenericTask>("yadoms.update",
                                                             [downloadUrl, expectedMd5Hash](auto taskProgressFunc)
                                                             {
                                                                worker::CYadoms::update(taskProgressFunc,
                                                                                        downloadUrl,
                                                                                        expectedMd5Hash);
                                                             }));
      return startTask(task);
   }

   bool CUpdateManager::checkDependencies(const boost::shared_ptr<shared::CDataContainer>& itemVersionNode)
   {
      if (!itemVersionNode->containsChild("dependencies"))
         return true;

      // For now, only Yadoms minimum version check is supported
      const shared::versioning::CSemVer minSupportedYadomsVersion(itemVersionNode->get<std::string>("dependencies.yadoms.minimumVersion"));
      const auto& actualYadomsVersion = shared::CServiceLocator::instance().get<IRunningInformation>()->getSoftwareVersion().getVersion();

      return actualYadomsVersion >= minSupportedYadomsVersion;
   }
} // namespace update
