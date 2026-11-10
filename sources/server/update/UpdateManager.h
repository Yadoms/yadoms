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
   class CUpdateManager final : public IUpdateManager
	{
	public:
      CUpdateManager(boost::shared_ptr<task::CScheduler> taskScheduler,
								boost::shared_ptr<pluginSystem::CManager> pluginManager,
								boost::shared_ptr<automation::interpreter::IManager> interpreterManager,
								boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
								bool developerMode,
								boost::shared_ptr<const IPathProvider> pathProvider);
      ~CUpdateManager() override;

		// IUpdateManager implementation
		boost::shared_ptr<shared::CDataContainer> getUpdates(bool includePrereleases) const override;
      void asyncScanForUpdates() override;
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
      void doWork();
      void getAvailableVersionsFromServer();
      static bool updateAvailable(const boost::shared_ptr<shared::CDataContainer>& updates,
                                  const std::string& componentTag);
      bool updateAvailable(bool includePrereleases) const;
      void scanForUpdates();
      boost::shared_ptr<shared::CDataContainer> buildUpdates(
         bool includePrereleases,
         const shared::versioning::CSemVer& yadomsLocalVersion,
         const boost::shared_ptr<shared::CDataContainer>& yadomsAvailableVersions,
         const pluginSystem::IFactory::AvailablePluginMap& pluginsLocalVersions,
         const boost::shared_ptr<shared::CDataContainer>& pluginsAvailableVersions,
         const worker::CWidget::AvailableWidgetMap& widgetsLocalVersions,
         const boost::shared_ptr<shared::CDataContainer>& widgetsAvailableVersions,
         const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& scriptInterpretersLocalVersions,
         const boost::shared_ptr<shared::CDataContainer>& scriptInterpretersAvailableVersions) const;
		boost::shared_ptr<shared::CDataContainer> buildYadomsList(const shared::versioning::CSemVer& localVersion,
                                                                const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                bool includePrereleases) const;
		boost::shared_ptr<shared::CDataContainer> buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                                const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                bool includePrereleases) const;
		boost::shared_ptr<shared::CDataContainer> buildWidgetList(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                                const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                bool includePrereleases) const;
		boost::shared_ptr<shared::CDataContainer> buildScriptInterpreterList(
			const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const boost::shared_ptr<shared::CDataContainer>& availableVersions,
			bool includePrereleases) const;
		boost::shared_ptr<shared::CDataContainer> addUpdateableYadoms(const shared::versioning::CSemVer& localVersion,
                                                                    const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                    bool includePrereleases) const;
		boost::shared_ptr<shared::CDataContainer> addUpdateablePlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                                     const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                     bool includePrereleases) const;
		static boost::shared_ptr<shared::CDataContainer> addNewPlugins(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                                     const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                     bool includePrereleases);
		boost::shared_ptr<shared::CDataContainer> addUpdateableWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                                     const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                     bool includePrereleases) const;
		static boost::shared_ptr<shared::CDataContainer> addNewWidgets(const worker::CWidget::AvailableWidgetMap& localVersions,
                                                                     const boost::shared_ptr<shared::CDataContainer>& availableVersions,
                                                                     bool includePrereleases);
		boost::shared_ptr<shared::CDataContainer> addUpdateableScriptInterpreters(
			const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const boost::shared_ptr<shared::CDataContainer>& availableVersions,
			bool includePrereleases) const;
		static boost::shared_ptr<shared::CDataContainer> addNewScriptInterpreters(
			const std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>>& localVersions,
         const boost::shared_ptr<shared::CDataContainer>& availableVersions,
			bool includePrereleases);
		static boost::shared_ptr<shared::CDataContainer> buildUpdateableVersionsNode(const std::string& installed,
                                                                                   std::map<std::string, boost::shared_ptr<shared::CDataContainer>>
                                                                                   older,
                                                                                   std::map<std::string, boost::shared_ptr<shared::CDataContainer>>
                                                                                   newer);
      static boost::shared_ptr<shared::CDataContainer> buildNewVersionsNode(
         const std::map<std::string, boost::shared_ptr<shared::CDataContainer>>& newItemAvailableVersions);
		void notifyNewUpdateAvailable() const;
      static bool checkDependencies(const boost::shared_ptr<shared::CDataContainer>& itemVersionNode);
      std::string findMd5HashAssociatedTo(const std::string& downloadUrl) const;

	private:
      bool startTask(const boost::shared_ptr<task::ITask>& task, std::string& taskUid) const;
      std::string startTask(const boost::shared_ptr<task::ITask>& task) const;


      const boost::shared_ptr<task::CScheduler> m_taskScheduler;


		boost::shared_ptr<pluginSystem::CManager> m_pluginManager;
		boost::shared_ptr<automation::interpreter::IManager> m_interpreterManager;

		boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;
		bool m_developerMode;
		boost::shared_ptr<const IPathProvider> m_pathProvider;

		boost::thread m_thread;
		shared::event::CEventHandler m_evtHandler;

		mutable boost::recursive_mutex m_updateMutex;
      boost::shared_ptr<shared::CDataContainer> m_yadomsAvailableVersions;
      boost::shared_ptr<shared::CDataContainer> m_pluginsAvailableVersions;
      boost::shared_ptr<shared::CDataContainer> m_widgetsAvailableVersions;
      boost::shared_ptr<shared::CDataContainer> m_scriptInterpretersAvailableVersions;
	};
} // namespace update
