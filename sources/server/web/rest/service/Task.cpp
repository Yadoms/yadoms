#include "stdafx.h"
#include "Task.h"
#include "web/poco/RestResult.h"
#include <utility>
#include "task/IInstance.h"
#include "task/Scheduler.h"
#include "task/ITask.h"
#include "web/poco/RestDispatcherHelpers.hpp"

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

         void CTask::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CTask::getAllTasks);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CTask::getOneTask);
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CTask::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            //TODO

            return m_endPoints;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CTask::getOneTask(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
         {
            if (parameters.size() > 1)
            {
               const auto taskUid = parameters[1];
               const auto taskFound = m_taskManager->getTask(taskUid);
               if (taskFound == nullptr)
                  return poco::CRestResult::GenerateError("Task not found");

               return poco::CRestResult::GenerateSuccess(serialize(taskFound));
            }
            return poco::CRestResult::GenerateError("Invalid parameter count (need guid of the task in url)");
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CTask::getAllTasks(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const
         {
            const auto taskList = m_taskManager->getAllTasks();

            shared::CDataContainer collection;
            collection.createArray("task");

            for (const auto& it : taskList)
               collection.appendArray("task", serialize(it));

            return poco::CRestResult::GenerateSuccess(collection);
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
