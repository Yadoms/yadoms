#pragma once
#include "task/Scheduler.h"
#include "pluginSystem/Manager.h"
#include "worker/IUpdateChecker.h"

namespace update
{
   //-----------------------------------------------------------------------------
   /// \brief  Provide methods for starting update and check for updates
   //-----------------------------------------------------------------------------   
   class CUpdateManager
   {
   public:
      //-----------------------------------------------------------------------------
      /// \brief  Constructor
      /// \param [in]   taskScheduler        The task scheduler
      /// \param [in]   pluginManager        Plugin manager (needed to update plugin list after update)
      //-----------------------------------------------------------------------------   
      CUpdateManager(boost::shared_ptr<task::CScheduler>& taskScheduler,
                     boost::shared_ptr<pluginSystem::CManager> pluginManager,
                     boost::shared_ptr<worker::IUpdateChecker> updateChecker);

      //-----------------------------------------------------------------------------
      /// \brief  Destructor
      //-----------------------------------------------------------------------------   
      virtual ~CUpdateManager();


      //-----------------------------------------------------------------------------
      /// \brief  Update a plugin (async process)
      /// \param [in]   pluginName        The plugin name to update
      /// \param [in]   downloadUrl       The plugin package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updatePluginAsync(const std::string& pluginName, const std::string& downloadUrl) const;

      //-----------------------------------------------------------------------------
      /// \brief  Install a plugin (async process)
      /// \param [in]   downloadUrl       The plugin package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string installPluginAsync(const std::string& downloadUrl) const;

      //-----------------------------------------------------------------------------
      /// \brief  Remove a plugin (async process)
      /// \param [in]   pluginName       The plugin name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string removePluginAsync(const std::string& pluginName) const;

      //-----------------------------------------------------------------------------
      /// \brief  Update a widget (async process)
      /// \param [in]   widgetName        The widget name to update
      /// \param [in]   downloadUrl       The widget package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updateWidgetAsync(const std::string& widgetName, const std::string& downloadUrl) const;

      //-----------------------------------------------------------------------------
      /// \brief  Install a widget (async process)
      /// \param [in]   downloadUrl       The widget package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string installWidgetAsync(const std::string& downloadUrl) const;

      //-----------------------------------------------------------------------------
      /// \brief  Remove a widget (async process)
      /// \param [in]   widgetName       The widget name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string removeWidgetAsync(const std::string& widgetName) const;


      //-----------------------------------------------------------------------------
      /// \brief  Update a scriptInterpreter (async process)
      /// \param [in]   scriptInterpreterName        The scriptInterpreter name to update
      /// \param [in]   downloadUrl       The scriptInterpreter package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updateScriptInterpreterAsync(const std::string& scriptInterpreterName, const std::string& downloadUrl) const;

      //-----------------------------------------------------------------------------
      /// \brief  Install a scriptInterpreter (async process)
      /// \param [in]   downloadUrl       The scriptInterpreter package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string installScriptInterpreterAsync(const std::string& downloadUrl) const;

      //-----------------------------------------------------------------------------
      /// \brief  Remove a scriptInterpreter (async process)
      /// \param [in]   scriptInterpreterName       The scriptInterpreter name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const;

      //-----------------------------------------------------------------------------
      /// \brief  Start an update of Yadoms (asynchronous; update result is provided by webservice)
      /// \param [in]   versionToInstall        The yadoms lastVersion.json to install (can be lastVersion.json, or any other one)
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      std::string updateYadomsAsync(const shared::CDataContainer& versionToInstall) const;

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \param [out]  taskUid     The task identifier created if sucessfully started
      /// \return result (true/false)
      //-----------------------------------------------------------------------------   
      bool startTask(boost::shared_ptr<task::ITask> task, std::string& taskUid) const;

      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \return The task identifier created if sucessfully started
      /// \throw shared::exception::CException if task launch fails
      //-----------------------------------------------------------------------------   
      std::string startTask(boost::shared_ptr<task::ITask> task) const;


      //-----------------------------------------------------------------------------
      /// \brief  Task scheduler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;

      //-----------------------------------------------------------------------------
      /// \brief  Plugin manager (needed to update plugin list after update)
      //-----------------------------------------------------------------------------
      boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

      boost::shared_ptr<worker::IUpdateChecker> m_updateChecker;
   };
} // namespace update
