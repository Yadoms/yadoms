#include "stdafx.h"
#include "UpdateManager.h"
#include <shared/Log.h>

#include "task/ITask.h"
#include "task/IInstance.h"

#include "task/update/Yadoms.h"
#include "task/update/Plugin.h"
#include "task/update/Widget.h"

#include "source/Yadoms.h"
#include "source/Plugin.h"
#include "source/Widget.h"

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


   bool CUpdateManager::checkForUpdateAsync(boost::shared_ptr<source::IUpdateSource> source)
   {
      bool result = false;

      if (source)
      {
         YADOMS_LOG(information) << "Check for update (async) " << source->getUpdateType() << " " << source->getInstalledVersion().toString();

         boost::shared_ptr<task::ITask> task;

         switch (source->getUpdateType())
         {
         case EUpdateType::kYadomsValue:
            YADOMS_LOG(information) << "Create a Yadoms check for update task";
            task.reset(new task::update::CYadoms(boost::dynamic_pointer_cast<source::CYadoms>(source), true));
            break;
         case EUpdateType::kPluginValue:
            YADOMS_LOG(information) << "Create a plugin check for update task";
            task.reset(new task::update::CPlugin(boost::dynamic_pointer_cast<source::CPlugin>(source), true));
            break;
         case EUpdateType::kWidgetValue:
            YADOMS_LOG(information) << "Create a widget check for update task";
            task.reset(new task::update::CWidget(boost::dynamic_pointer_cast<source::CWidget>(source), true));
            break;
         }
         result = startTask(task);
      }
      else
      {
         YADOMS_LOG(error) << "The check for update source must not be NULL";
      }
      return result;
   }

   bool CUpdateManager::updateAsync(boost::shared_ptr<source::IUpdateSource> source)
   {

      bool result = false;

      if (source)
      {
         YADOMS_LOG(information) << "Update (async) " << source->getUpdateType() << " " << source->getInstalledVersion().toString();

         boost::shared_ptr<task::ITask> task;

         switch (source->getUpdateType())
         {
         case EUpdateType::kYadomsValue:
            YADOMS_LOG(information) << "Create a Yadoms update task";
            task.reset(new task::update::CYadoms(boost::dynamic_pointer_cast<source::CYadoms>(source), false));
            break;
         case EUpdateType::kPluginValue:
            YADOMS_LOG(information) << "Create a plugin update task";
            task.reset(new task::update::CPlugin(boost::dynamic_pointer_cast<source::CPlugin>(source), false));
            break;
         case EUpdateType::kWidgetValue:
            YADOMS_LOG(information) << "Create a widget update task";
            task.reset(new task::update::CWidget(boost::dynamic_pointer_cast<source::CWidget>(source), false));
            break;
         }
         result = startTask(task);
      }
      else
      {
         YADOMS_LOG(error) << "The update source must not be NULL";
      }
      return result;
   }
  
} // namespace update
