#pragma once
#include "IUpdateManager.h"
#include "task/Scheduler.h"
#include "pluginSystem/Manager.h"
#include "worker/Widget.h"
#include "automation/interpreter/IManager.h"

namespace shared
{
   namespace versioning
   {
      class CVersionInformation;
   }
}

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
      shared::CDataContainer getUpdates(bool includePreleases) const override;
      // [END] IUpdateManager implementation

      //-----------------------------------------------------------------------------
      /// \brief  Update a plugin (async process)
      /// \param [in]   pluginName        The plugin name to update
      /// \param [in]   downloadUrl       The plugin package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updatePluginAsync(const std::string& pluginName, const std::string& downloadUrl) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Install a plugin (async process)
      /// \param [in]   downloadUrl       The plugin package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string installPluginAsync(const std::string& downloadUrl) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Remove a plugin (async process)
      /// \param [in]   pluginName       The plugin name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string removePluginAsync(const std::string& pluginName) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Update a widget (async process)
      /// \param [in]   widgetName        The widget name to update
      /// \param [in]   downloadUrl       The widget package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updateWidgetAsync(const std::string& widgetName, const std::string& downloadUrl) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Install a widget (async process)
      /// \param [in]   downloadUrl       The widget package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string installWidgetAsync(const std::string& downloadUrl) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Remove a widget (async process)
      /// \param [in]   widgetName       The widget name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string removeWidgetAsync(const std::string& widgetName) const override;


      //-----------------------------------------------------------------------------
      /// \brief  Update a scriptInterpreter (async process)
      /// \param [in]   scriptInterpreterName        The scriptInterpreter name to update
      /// \param [in]   downloadUrl       The scriptInterpreter package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updateScriptInterpreterAsync(const std::string& scriptInterpreterName, const std::string& downloadUrl) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Install a scriptInterpreter (async process)
      /// \param [in]   downloadUrl       The scriptInterpreter package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string installScriptInterpreterAsync(const std::string& downloadUrl) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Remove a scriptInterpreter (async process)
      /// \param [in]   scriptInterpreterName       The scriptInterpreter name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const override;

      //-----------------------------------------------------------------------------
      /// \brief  Start an update of Yadoms (asynchronous; update result is provided by webservice)
      /// \param [in]   versionToInstall        The yadoms lastVersion.json to install (can be lastVersion.json, or any other one)
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updateYadomsAsync(const shared::CDataContainer& versionToInstall) const override;

   protected:
      void doWork(const boost::posix_time::time_duration scanPeriod);
      bool scan();
      void scanForUpdates(worker::CWorkerTools::WorkerProgressFunc progressCallback);
      shared::CDataContainer buildUpdates(bool includePreleases,
                                          const shared::versioning::CVersion& yadomsLocalVersion,
                                          const shared::CDataContainer& yadomsAvailableVersions,
                                          const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
                                          const shared::CDataContainer& pluginsAvailableVersions,
                                          const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
                                          const shared::CDataContainer& widgetsAvailableVersions,
                                          const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>&
                                          scriptInterpretersLocalVersions,
                                          const shared::CDataContainer& scriptInterpretersAvailableVersions);
      static shared::CDataContainer buildYadomsList(const shared::versioning::CVersion& localVersion,
                                                    const shared::CDataContainer& availableVersions,
                                                    bool includePreleases);
      shared::CDataContainer buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                             const shared::CDataContainer& availableVersions,
                                             bool includePreleases);
      shared::CDataContainer buildWidgetList(const worker::CWidget::AvailableWidgetMap& localVersions,
                                             const shared::CDataContainer& availableVersions,
                                             bool includePreleases);
      shared::CDataContainer buildScriptInterpreterList(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePreleases);
      static shared::CDataContainer addUpdatableYadoms(const shared::versioning::CVersion& localVersion,
                                                       const shared::CDataContainer& availableVersions,
                                                       bool includePreleases);
      shared::CDataContainer addUpdatablePlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                 const shared::CDataContainer& availableVersions,
                                                 bool includePreleases) const;
      shared::CDataContainer addNewPlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                           const shared::CDataContainer& availableVersions,
                                           bool includePreleases);
      shared::CDataContainer addUpdatableWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                 const shared::CDataContainer& availableVersions,
                                                 bool includePreleases) const;
      shared::CDataContainer addNewWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                           const shared::CDataContainer& availableVersions,
                                           bool includePreleases);
      shared::CDataContainer addUpdatableScriptInterpreters(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePreleases) const;
      shared::CDataContainer addNewScriptInterpreters(
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const shared::CDataContainer& availableVersions,
         bool includePreleases);
      static shared::CDataContainer buildUpdatableVersionsNode(const std::string& installed,
                                                               std::map<std::string, shared::CDataContainer> older,
                                                               std::map<std::string, shared::CDataContainer> newer);
      static shared::CDataContainer buildNewVersionsNode(const std::map<std::string, shared::CDataContainer>& newItemAvailableVersions);
      void notifyNewUpdateAvailable() const;

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \param [out]  taskUid     The task identifier created if sucessfully started
      /// \return result (true/false)
      //-----------------------------------------------------------------------------   
      bool startTask(boost::shared_ptr<task::ITask> task, std::string& taskUid) const;//TODO utilisé ?

      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \return The task identifier created if sucessfully started
      /// \throw shared::exception::CException if task launch fails
      //-----------------------------------------------------------------------------   
      std::string startTask(boost::shared_ptr<task::ITask> task) const;//TODO utilisé ?


      //-----------------------------------------------------------------------------
      /// \brief  Task scheduler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;//TODO utilisé ?


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
