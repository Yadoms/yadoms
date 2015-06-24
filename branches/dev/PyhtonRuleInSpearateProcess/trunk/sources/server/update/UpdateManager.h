#pragma once
#include "task/Scheduler.h"
#include <shared/DataContainer.h>

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
      //-----------------------------------------------------------------------------   
      CUpdateManager(boost::shared_ptr<task::CScheduler> & taskScheduler);

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
      const std::string updatePluginAsync(const std::string & pluginName, const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Install a plugin (async process)
      /// \param [in]   downloadUrl       The plugin package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string installPluginAsync(const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Remove a plugin (async process)
      /// \param [in]   pluginName       The plugin name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string removePluginAsync(const std::string & pluginName);

      //-----------------------------------------------------------------------------
      /// \brief  Update a widget (async process)
      /// \param [in]   widgetName        The widget name to update
      /// \param [in]   downloadUrl       The widget package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string updateWidgetAsync(const std::string & widgetName, const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Install a widget (async process)
      /// \param [in]   downloadUrl       The widget package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string installWidgetAsync(const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Remove a widget (async process)
      /// \param [in]   widgetName       The widget name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string removeWidgetAsync(const std::string & widgetName);
      

      //-----------------------------------------------------------------------------
      /// \brief  Update a scriptInterpreter (async process)
      /// \param [in]   scriptInterpreterName        The scriptInterpreter name to update
      /// \param [in]   downloadUrl       The scriptInterpreter package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string updateScriptInterpreterAsync(const std::string & scriptInterpreterName, const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Install a scriptInterpreter (async process)
      /// \param [in]   downloadUrl       The scriptInterpreter package download url
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string installScriptInterpreterAsync(const std::string & downloadUrl);

      //-----------------------------------------------------------------------------
      /// \brief  Remove a scriptInterpreter (async process)
      /// \param [in]   scriptInterpreterName       The scriptInterpreter name to remove
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string removeScriptInterpreterAsync(const std::string & scriptInterpreterName);


      //-----------------------------------------------------------------------------
      /// \brief  Start a check for update for Yadoms (asynchronous; check for update result is provided by webservice)
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string checkForYadomsUpdateAsync();

      //-----------------------------------------------------------------------------
      /// \brief  Start an update of Yadoms (asynchronous; update result is provided by webservice)
      /// \param [in]   versionToInstall        The yadoms lastVersion.json to install (can be lastVersion.json, or any other one)
      /// \return  The task unique id
      //-----------------------------------------------------------------------------   
      const std::string updateYadomsAsync(const shared::CDataContainer & versionToInstall);

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Worker for checking for Yadoms update
      /// \param [in]   progressCallback        The callback to use for reporting progression
      //-----------------------------------------------------------------------------   
      void checkForYadomsUpdateTaskWorker(task::ITask::TaskProgressFunc progressCallback);

      //-----------------------------------------------------------------------------
      /// \brief  Worker for updating Yadoms
      /// \param [in]   progressCallback        The callback to use for reporting progression
      /// \param [in]   versionToInstall        The version to install (dont use reference but copy)
      //-----------------------------------------------------------------------------   
      void updateYadomsTaskWorker(task::ITask::TaskProgressFunc progressCallback, shared::CDataContainer versionToInstall);
      
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \param [out]  taskUid     The task identifier created if sucessfully started
      /// \return result (true/false)
      //-----------------------------------------------------------------------------   
      bool startTask(boost::shared_ptr<task::ITask> task, std::string & taskUid); 
      
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \return The task identifier created if sucessfully started
      /// \throw shared::exception::CException if task launch fails
      //-----------------------------------------------------------------------------   
      const std::string startTask(boost::shared_ptr<task::ITask> task);

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Task scheduler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;
   };
   
} // namespace update
