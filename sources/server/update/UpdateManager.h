#pragma once
#include "IUpdateManager.h"
#include "task/Scheduler.h"
#include "pluginSystem/Manager.h"
#include "worker/Widget.h"
#include "automation/interpreter/IManager.h"


namespace update
{
   //-----------------------------------------------------------------------------
   /// \brief  Provide methods for starting update and check for updates
   //-----------------------------------------------------------------------------   
   class CUpdateManager : public IUpdateManager
   {
   public:
      CUpdateManager(boost::shared_ptr<task::CScheduler>& taskScheduler,
                     boost::shared_ptr<pluginSystem::CManager> pluginManager,
                     boost::shared_ptr<automation::interpreter::IManager> interpreterManager,
                     boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                     bool developerMode,
                     boost::shared_ptr<const IPathProvider> pathProvider);
      virtual ~CUpdateManager();

      // IUpdateManager implementation
      std::string scanForUpdatesAsync() override;
      shared::CDataContainerSharedPtr getUpdates(bool includePrereleases) const override;
      std::string updatePluginAsync(const std::string& pluginName, const std::string& downloadUrl) const override;
      std::string installPluginAsync(const std::string& downloadUrl) const override;
      std::string removePluginAsync(const std::string& pluginName) const override;
      std::string updateWidgetAsync(const std::string& widgetName, const std::string& downloadUrl) const override;
      std::string installWidgetAsync(const std::string& downloadUrl) const override;
      std::string removeWidgetAsync(const std::string& widgetName) const override;
      std::string updateScriptInterpreterAsync(const std::string& scriptInterpreterName, const std::string& downloadUrl) const override;
      std::string installScriptInterpreterAsync(const std::string& downloadUrl) const override;
      std::string removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const override;
      std::string updateYadomsAsync(const std::string& downloadUrl) const override;
      // [END] IUpdateManager implementation


   protected:
      void doWork(const boost::posix_time::time_duration& scanPeriod);
      bool scan(boost::shared_ptr<const IPathProvider> pathProvider);
      void scanForUpdates(worker::CWorkerTools::WorkerProgressFunc progress_callback);
      shared::CDataContainerSharedPtr buildUpdates(bool includePrereleases,
                                          const shared::versioning::CSemVer& yadomsLocalVersion,
                                          shared::CDataContainerSharedPtr yadomsAvailableVersions,
                                          const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
                                          shared::CDataContainerSharedPtr pluginsAvailableVersions,
                                          const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
                                          shared::CDataContainerSharedPtr widgetsAvailableVersions,
                                          const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
                                          scriptInterpretersLocalVersions,
                                          shared::CDataContainerSharedPtr scriptInterpretersAvailableVersions) const;
      shared::CDataContainerSharedPtr buildYadomsList(const shared::versioning::CSemVer& localVersion,
                                             shared::CDataContainerSharedPtr availableVersions,
                                             bool includePrereleases) const;
      shared::CDataContainerSharedPtr buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                             shared::CDataContainerSharedPtr available_versions,
                                             bool includePrereleases) const;
      shared::CDataContainerSharedPtr buildWidgetList(const worker::CWidget::AvailableWidgetMap& localVersions,
                                             shared::CDataContainerSharedPtr available_versions,
                                             bool includePrereleases) const;
      shared::CDataContainerSharedPtr buildScriptInterpreterList(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         shared::CDataContainerSharedPtr available_versions,
         bool includePrereleases) const;
      shared::CDataContainerSharedPtr addUpdateableYadoms(const shared::versioning::CSemVer& localVersion,
                                                 shared::CDataContainerSharedPtr available_versions,
                                                 bool includePrereleases) const;
      shared::CDataContainerSharedPtr addUpdateablePlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                  shared::CDataContainerSharedPtr available_versions,
                                                  bool includePrereleases) const;
      static shared::CDataContainerSharedPtr addNewPlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                  shared::CDataContainerSharedPtr available_versions,
                                                  bool includePrereleases);
      shared::CDataContainerSharedPtr addUpdateableWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                  shared::CDataContainerSharedPtr available_versions,
                                                  bool includePrereleases) const;
      static shared::CDataContainerSharedPtr addNewWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                  shared::CDataContainerSharedPtr available_versions,
                                                  bool includePrereleases);
      shared::CDataContainerSharedPtr addUpdateableScriptInterpreters(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         shared::CDataContainerSharedPtr available_versions,
         bool includePrereleases) const;
      static shared::CDataContainerSharedPtr addNewScriptInterpreters(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         shared::CDataContainerSharedPtr available_versions,
         bool includePrereleases);
      static shared::CDataContainerSharedPtr buildUpdateableVersionsNode(const std::string& installed,
                                                                std::map<std::string, shared::CDataContainerSharedPtr> older,
                                                                std::map<std::string, shared::CDataContainerSharedPtr> newer);
      static shared::CDataContainerSharedPtr buildNewVersionsNode(const std::map<std::string, shared::CDataContainerSharedPtr>& newItemAvailableVersions);
      void notifyNewUpdateAvailable() const;
      static bool checkDependencies(shared::CDataContainerSharedPtr item_version_node);
      std::string findMd5HashAssociatedTo(const std::string& downloadUrl,
                                          const std::string& allUpdatesNode) const;

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \param [out]  taskUid     The task identifier created if successfully started
      /// \return result (true/false)
      //-----------------------------------------------------------------------------   
      bool startTask(boost::shared_ptr<task::ITask> task, std::string& taskUid) const;

      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \return The task identifier created if successfully started
      /// \throw shared::exception::CException if task launch fails
      //-----------------------------------------------------------------------------   
      std::string startTask(boost::shared_ptr<task::ITask> task) const;


      //-----------------------------------------------------------------------------
      /// \brief  Task scheduler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;


      boost::shared_ptr<pluginSystem::CManager> m_pluginManager;
      boost::shared_ptr<automation::interpreter::IManager> m_interpreterManager;

      boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;
      bool m_developerMode;
      boost::shared_ptr<const IPathProvider> m_pathProvider;

      boost::thread m_thread;
      shared::event::CEventHandler m_evtHandler;

      mutable boost::recursive_mutex m_updateMutex;
      shared::CDataContainerSharedPtr m_allUpdates;
      shared::CDataContainerSharedPtr m_releasesOnlyUpdates;
   };
} // namespace update
