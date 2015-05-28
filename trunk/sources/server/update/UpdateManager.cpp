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

namespace update
{
   CUpdateManager::CUpdateManager(boost::shared_ptr<task::CScheduler> & taskScheduler)
      :m_taskScheduler(taskScheduler)
   {
   }

   CUpdateManager::~CUpdateManager()
   {

   }

   bool CUpdateManager::startTask(boost::shared_ptr<task::ITask> task)
   {
      bool result = false;
      if (task)
      {
         std::string taskUid;
         result = m_taskScheduler->runTask(task, taskUid);

         if (result)
            YADOMS_LOG(information) << "Check for update task started : " << taskUid;
      }
      else
      {
         YADOMS_LOG(error) << "The task is not known";
      }
      return result;
   }



   void CUpdateManager::updatePluginAsync(const std::string & pluginName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CPluginUpdate(pluginName, downloadUrl));
      startTask(task);
   }
  
   void CUpdateManager::installPluginAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CPluginInstall(downloadUrl));
      startTask(task);
   }

   void CUpdateManager::removePluginAsync(const std::string & pluginName)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CPluginRemove(pluginName));
      startTask(task);
   }
   void CUpdateManager::updateWidgetAsync(const std::string & widgetName, const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CWidgetUpdate(widgetName, downloadUrl));
      startTask(task);
   }
  
   void CUpdateManager::installWidgetAsync(const std::string & downloadUrl)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CWidgetInstall(downloadUrl));
      startTask(task);
   }

   void CUpdateManager::removeWidgetAsync(const std::string & widgetName)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CWidgetRemove(widgetName));
      startTask(task);
   }


   void CUpdateManager::checkForYadomsUpdateAsync()
   {
      boost::shared_ptr<task::ITask> task(new task::update::CYadomsCheck());
      startTask(task);
   }

   void CUpdateManager::updateYadomsAsync(const shared::CDataContainer & versionToInstall)
   {
      boost::shared_ptr<task::ITask> task(new task::update::CYadomsUpdate(versionToInstall));
      startTask(task);
   }

   

  
} // namespace update
