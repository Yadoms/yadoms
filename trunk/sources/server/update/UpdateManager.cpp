#include "stdafx.h"
#include "UpdateManager.h"
#include <shared/Log.h>

#include "task/ITask.h"
#include "task/IInstance.h"

#include "task/update/YadomsCheck.h"
#include "task/update/YadomsUpdate.h"
#include "task/update/PluginInstall.h"
#include "task/update/PluginRemove.h"
#include "task/update/PluginUpdate.h"
#include "task/update/WidgetInstall.h"
#include "task/update/WidgetRemove.h"
#include "task/update/WidgetUpdate.h"
#include "task/update/ScriptInterpreterInstall.h"
#include "task/update/ScriptInterpreterRemove.h"
#include "task/update/ScriptInterpreterUpdate.h"
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
      boost::shared_ptr<task::ITask> task(new task::update::CPluginUpdate(pluginName, downloadUrl));
      return startTask(task);
   }
  
   const std::string CUpdateManager::installPluginAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CPluginInstall(downloadUrl));
      return startTask(task);
   }

   const std::string CUpdateManager::removePluginAsync(const std::string & pluginName)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CPluginRemove(pluginName));
      return startTask(task);
   }

   const std::string CUpdateManager::updateWidgetAsync(const std::string & widgetName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CWidgetUpdate(widgetName, downloadUrl));
      return startTask(task);
   }
  
   const std::string CUpdateManager::installWidgetAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CWidgetInstall(downloadUrl));
      return startTask(task);
   }

   const std::string CUpdateManager::removeWidgetAsync(const std::string & widgetName)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CWidgetRemove(widgetName));
      return startTask(task);
   }


   const std::string CUpdateManager::updateScriptInterpreterAsync(const std::string & scriptInterpreterName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CScriptInterpreterUpdate(scriptInterpreterName, downloadUrl));
      return startTask(task);
   }

   const std::string CUpdateManager::installScriptInterpreterAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CScriptInterpreterInstall(downloadUrl));
      return startTask(task);
   }

   const std::string CUpdateManager::removeScriptInterpreterAsync(const std::string & scriptInterpreterName)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CScriptInterpreterRemove(scriptInterpreterName));
      return startTask(task);
   }

   const std::string CUpdateManager::checkForYadomsUpdateAsync()
   {
      boost::shared_ptr<task::ITask> task(new task::update::CYadomsCheck());
      return startTask(task);
   }

   const std::string CUpdateManager::updateYadomsAsync(const shared::CDataContainer & versionToInstall)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CYadomsUpdate(versionToInstall));
      return startTask(task);
   }

   

  
} // namespace update
