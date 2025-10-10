#include "stdafx.h"
#include "Task.h"
#include <utility>
#include "RestEndPoint.h"
#include "task/IInstance.h"
#include "task/Scheduler.h"
#include "web/poco/RestDispatcherHelpers.hpp"
#include "web/poco/RestResult.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"

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
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CTask::getAllTasks)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CTask::getOneTask)
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CTask::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "tasks", getTasksV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "tasks/{uuids}/result", getTasksResultsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "tasks/{uuids}", getTasksV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "tasks/{uuid}/result", deleteTaskResultV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "tasks/{uuid}", deleteTaskV2));

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

         boost::shared_ptr<IAnswer> CTask::getTasksV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto taskIds = request->pathVariableExists("uuids")
                                       ? request->pathVariableAsList("uuids")
                                       : nullptr;

               // Process the request
               auto tasks = taskIds ? m_taskManager->getTasks(*taskIds) : m_taskManager->getAllTasks();

               // Filtering
               const auto fromName = request->queryParam("from-name", std::string());

               const auto fromStatus = request->queryParamExists("from-status")
                                          ? boost::make_optional(task::ETaskStatus(request->queryParam("from-status")))
                                          : boost::optional<task::ETaskStatus>();

               tasks.erase(std::remove_if(tasks.begin(),
                                          tasks.end(),
                                          [this, &fromName, &fromStatus](const auto& task)
                                          {
                                             if (!fromName.empty() && fromName != task->getName())
                                                return true;

                                             if (fromStatus && *fromStatus != task->getStatus())
                                                return true;

                                             return false;
                                          }), tasks.end());

               if (tasks.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> taskEntries;
               for (const auto& task : tasks)
               {
                  auto taskEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("uuid") != props->end())
                     taskEntry->set("uuid", task->getGuid());
                  if (props->empty() || props->find("name") != props->end())
                     taskEntry->set("name", task->getName());
                  if (props->empty() || props->find("progression") != props->end())
                     taskEntry->set("progression", task->getProgression());
                  if (props->empty() || props->find("message") != props->end())
                     taskEntry->set("message", task->getMessage());
                  if (props->empty() || props->find("status") != props->end())
                     taskEntry->set("status", task->getStatus());
                  if (props->empty() || props->find("creationDate") != props->end())
                     taskEntry->set("creationDate", task->getCreationDate());

                  taskEntries.push_back(taskEntry);
               }

               if (taskEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               if (taskIds && taskIds->size() == 1)
                  return CHelpers::getLongRunningOperationAnswer(*tasks.begin(),
                                                                 taskEntries.at(0));

               shared::CDataContainer container;
               container.set("tasks", taskEntries);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getTasks request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getTasks request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get tasks");
            }
         }

         boost::shared_ptr<IAnswer> CTask::deleteTaskV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto uuid = request->pathVariable("uuid", std::string());
               if (uuid.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "task uuid was not provided");

               const auto task = m_taskManager->getTask(uuid);
               if (task == nullptr)
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "Unknown task uuid");

               task->cancel(task::IInstance::NoWait);

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete task");
            }
         }

         boost::shared_ptr<IAnswer> CTask::getTasksResultsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto taskIds = request->pathVariableExists("uuids")
                                       ? request->pathVariableAsList("uuids")
                                       : nullptr;

               // Process the request
               auto tasks = taskIds ? m_taskManager->getTasks(*taskIds) : m_taskManager->getAllTasks();

               // Filtering
               const auto fromName = request->queryParam("from-name", std::string());

               tasks.erase(std::remove_if(tasks.begin(),
                                          tasks.end(),
                                          [this, &fromName](const auto& task)
                                          {
                                             if (!fromName.empty() && fromName != task->getName())
                                                return true;

                                             if (task->getStatus() == task::ETaskStatus::kStarted)
                                                return true;

                                             return false;
                                          }), tasks.end());

               if (tasks.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> taskEntries;
               for (const auto& task : tasks)
               {
                  auto taskEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("uuid") != props->end())
                     taskEntry->set("uuid", task->getGuid());
                  if (props->empty() || props->find("name") != props->end())
                     taskEntry->set("name", task->getName());
                  if (props->empty() || props->find("progression") != props->end())
                     taskEntry->set("progression", task->getProgression());
                  if (props->empty() || props->find("message") != props->end())
                     taskEntry->set("message", task->getMessage());
                  if (props->empty() || props->find("status") != props->end())
                     taskEntry->set("status", task->getStatus());
                  if (props->empty() || props->find("creationDate") != props->end())
                     taskEntry->set("creationDate", task->getCreationDate());

                  taskEntries.push_back(taskEntry);
               }

               return CHelpers::formatGetMultiItemsAnswer(taskIds->size() == 1,
                                                          taskEntries,
                                                          "tasks");
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getTasksResults request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getTasksResults request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get tasks results");
            }
         }

         boost::shared_ptr<IAnswer> CTask::deleteTaskResultV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto uuid = request->pathVariable("uuid", std::string());
               if (uuid.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "task uuid was not provided");

               m_taskManager->deleteTask(uuid);

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::invalid_argument&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                       "Fail to delete task");
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete task result");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
