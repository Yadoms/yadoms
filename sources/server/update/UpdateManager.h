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
      shared::CDataContainer getUpdates(bool includePrereleases) const override;
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
      void scanForUpdates(worker::CWorkerTools::WorkerProgressFunc progressCallback);
      shared::CDataContainer buildUpdates(bool includePrereleases,
                                          const shared::versioning::CSemVer& yadomsLocalVersion,
                                          const shared::CDataContainer& yadomsAvailableVersions,
                                          const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
                                          const shared::CDataContainer& pluginsAvailableVersions,
                                          const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
                                          const shared::CDataContainer& widgetsAvailableVersions,
                                          const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
                                          scriptInterpretersLocalVersions,
                                          const shared::CDataContainer& scriptInterpretersAvailableVersions) const;
      shared::CDataContainer buildYadomsList(const shared::versioning::CSemVer& localVersion,
                                             const shared::CDataContainer& availableVersions,
                                             bool includePrereleases) const;
      shared::CDataContainer buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                             const shared::CDataContainer& availableVersions,
                                             bool includePrereleases) const;
      shared::CDataContainer buildWidgetList(const worker::CWidget::AvailableWidgetMap& localVersions,
                                             const shared::CDataContainer& availableVersions,
                                             bool includePrereleases) const;
      shared::CDataContainer buildScriptInterpreterList(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePrereleases) const;
      shared::CDataContainer addUpdateableYadoms(const shared::versioning::CSemVer& localVersion,
                                                 const shared::CDataContainer& availableVersions,
                                                 bool includePrereleases) const;
      shared::CDataContainer addUpdateablePlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                  const shared::CDataContainer& availableVersions,
                                                  bool includePrereleases) const;
      static shared::CDataContainer addNewPlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                  const shared::CDataContainer& availableVersions,
                                                  bool includePrereleases);
      shared::CDataContainer addUpdateableWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                  const shared::CDataContainer& availableVersions,
                                                  bool includePrereleases) const;
      static shared::CDataContainer addNewWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                  const shared::CDataContainer& availableVersions,
                                                  bool includePrereleases);
      shared::CDataContainer addUpdateableScriptInterpreters(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePrereleases) const;
      static shared::CDataContainer addNewScriptInterpreters(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePrereleases);
      static shared::CDataContainer buildUpdateableVersionsNode(const std::string& installed,
                                                                std::map<std::string, shared::CDataContainer> older,
                                                                std::map<std::string, shared::CDataContainer> newer);
      static shared::CDataContainer buildNewVersionsNode(const std::map<std::string, shared::CDataContainer>& newItemAvailableVersions);
      void notifyNewUpdateAvailable() const;
      static bool checkDependencies(const shared::CDataContainer& itemVersionNode);
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
      shared::CDataContainer m_allUpdates;
      shared::CDataContainer m_releasesOnlyUpdates;
   };
} // namespace update
