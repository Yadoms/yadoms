#include "stdafx.h"
#include "Task.h"
#include <shared/exception/NotImplemented.hpp>
#include <utility>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "task/IInstance.h"
#include "task/Scheduler.h"
#include "task/ITask.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CTask::m_restKeyword = std::string("task");

         CTask::CTask(boost::shared_ptr<task::CScheduler> taskManager)
            : m_taskManager(std::move(taskManager))
         {
         }

         const std::string& CTask::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CTask::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CTask::getAllTasks);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CTask::getOneTask);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CTask::getOneTask(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
         {
            if (parameters.size() > 1)
            {
               const auto taskUid = parameters[1];
               const auto taskFound = m_taskManager->getTask(taskUid);
               if (taskFound == nullptr)
                  return CResult::GenerateError("Task not found");

               return CResult::GenerateSuccess(serialize(taskFound));
            }
            return CResult::GenerateError("Invalid parameter count (need guid of the task in url)");
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CTask::getAllTasks(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const
         {
            const auto taskList = m_taskManager->getAllTasks();

            shared::CDataContainer collection;
            collection.createArray("task");

            for (const auto& it : taskList)
               collection.appendArray("task", serialize(it));

            return CResult::GenerateSuccess(collection);
         }

         boost::shared_ptr<shared::CDataContainer> CTask::serialize(const boost::shared_ptr<const task::IInstance>& instance) const
         {
            auto container = shared::CDataContainer::make();
            container->set("uuid", instance->getGuid());
            container->set("name", instance->getName());
            container->set("progression", instance->getProgression());
            container->set("message", instance->getMessage());
            container->set("status", instance->getStatus());
            container->set("creationDate", instance->getCreationDate());
            return container;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
