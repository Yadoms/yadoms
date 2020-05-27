#include "stdafx.h"
#include "Task.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "task/IInstance.h"
#include "task/Scheduler.h"
#include "task/ITask.h"
#include "task/TaskFactory.h"

namespace web { namespace rest { namespace service {

   std::string CTask::m_restKeyword = std::string("task");

   CTask::CTask(boost::shared_ptr<task::CScheduler> taskManager)
      :m_taskManager(taskManager)
   {
   }


   CTask::~CTask()
   {
   }

   const std::string & CTask::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CTask::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CTask::getAllTasks);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CTask::getOneTask);
      
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword), CTask::addTask);
   }

   boost::shared_ptr<shared::serialization::IDataSerializable> CTask::getOneTask(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      if(parameters.size()>1)
      {
         std::string taskUid = parameters[1];
         boost::shared_ptr<task::IInstance> taskFound = m_taskManager->getTask(taskUid);
         if (taskFound == NULL)
            return CResult::GenerateError("Task not found");

         return CResult::GenerateSuccess(serialize(taskFound));
      }
      else
      {
         return CResult::GenerateError("Invalid parameter count (need guid of the task in url)");
      }
   }

   boost::shared_ptr<shared::serialization::IDataSerializable> CTask::getAllTasks(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      std::vector< boost::shared_ptr< task::IInstance > > taskList = m_taskManager->getAllTasks();

      //we build the collection answer
      shared::CDataContainer collection;
      collection.createArray("task");
      
      std::vector< boost::shared_ptr< task::IInstance > >::iterator it;
      for (it = taskList.begin(); it != taskList.end(); ++it)
      {
         collection.appendArray("task", serialize(*it));
      }

      return CResult::GenerateSuccess(collection);
   }

   boost::shared_ptr<shared::serialization::IDataSerializable> CTask::addTask(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         std::string type = shared::CDataContainer(requestContent).get<std::string>("type");
         
         boost::shared_ptr<task::ITask> t = task::CTaskFactory::createTask(type);

         if (t == NULL)
         {
            return CResult::GenerateError("Type of task doesn't exist: " + type);
         }

         //the task has been created we ask to run it

         std::string taskUid;
         m_taskManager->runTask(t, taskUid);

         boost::shared_ptr<task::IInstance> taskFound = m_taskManager->getTask(taskUid);
         if (taskFound == NULL)
            return CResult::GenerateError("Task not found");

         return CResult::GenerateSuccess(serialize(taskFound));
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in creating a new Task");
      }
   }

   boost::shared_ptr<shared::CDataContainer> CTask::serialize(boost::shared_ptr<task::IInstance> instance) const
   {
      boost::shared_ptr<shared::CDataContainer> container = shared::CDataContainer::make();
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
