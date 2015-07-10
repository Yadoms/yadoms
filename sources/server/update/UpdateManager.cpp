#include "stdafx.h"
#include "UpdateManager.h"
#include <shared/Log.h>

#include "task/GenericTask.h"

#include "worker/Yadoms.h"
#include "worker/Plugin.h"
#include "worker/Widget.h"
#include "worker/ScriptInterpreter.h"


namespace update
{
   CUpdateManager::CUpdateManager(boost::shared_ptr<task::CScheduler> & taskScheduler)
      :m_taskScheduler(taskScheduler)
   {
   }

   CUpdateManager::~CUpdateManager()
   {

   }

   bool CUpdateManager::startTask(boost::shared_ptr<task::ITask> task, std::string & taskUid)
   {
      bool result = false;
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

   const std::string CUpdateManager::startTask(boost::shared_ptr<task::ITask> task)
   {
      std::string taskId = "";
      if (startTask(task, taskId))
         return taskId;
      throw shared::exception::CException("Fail to start task");
   }


   const std::string CUpdateManager::updatePluginAsync(const std::string & pluginName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("plugin.update",
         boost::bind(&worker::CPlugin::update, _1, std::string(pluginName), std::string(downloadUrl)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }
  
   const std::string CUpdateManager::installPluginAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("plugin.install",
         boost::bind(&worker::CPlugin::install, _1, std::string(downloadUrl)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   const std::string CUpdateManager::removePluginAsync(const std::string & pluginName)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("plugin.remove",
         boost::bind(&worker::CPlugin::remove, _1, std::string(pluginName)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   const std::string CUpdateManager::updateWidgetAsync(const std::string & widgetName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("widget.update",
         boost::bind(&worker::CWidget::update, _1, std::string(widgetName), std::string(downloadUrl)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }
  
   const std::string CUpdateManager::installWidgetAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("widget.install",
      boost::bind(&worker::CWidget::install, _1, std::string(downloadUrl)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   const std::string CUpdateManager::removeWidgetAsync(const std::string & widgetName)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("widget.remove",
      boost::bind(&worker::CWidget::remove, _1, std::string(widgetName)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }


   const std::string CUpdateManager::updateScriptInterpreterAsync(const std::string & scriptInterpreterName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("scriptInterpreter.update",
         boost::bind(&worker::CScriptInterpreter::update, _1, std::string(scriptInterpreterName), std::string(downloadUrl)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   const std::string CUpdateManager::installScriptInterpreterAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("scriptInterpreter.install",
         boost::bind(&worker::CScriptInterpreter::install, _1, std::string(downloadUrl)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   const std::string CUpdateManager::removeScriptInterpreterAsync(const std::string & scriptInterpreterName)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("scriptInterpreter.remove",
         boost::bind(&worker::CScriptInterpreter::install, _1, std::string(scriptInterpreterName)))); //force to copy parameters because references cannot be used in async task
      return startTask(task);
   }

   const std::string CUpdateManager::updateYadomsAsync(const shared::CDataContainer & versionToInstall)
   {
      boost::shared_ptr<task::ITask> task(new task::CGenericTask("yadoms.update",
         boost::bind(&worker::CYadoms::update, _1, shared::CDataContainer(versionToInstall)))); //force to copy parameter because the versionToInstall is a reference and cannot be used "as is" in async task
      return startTask(task);
   }


  
} // namespace update
