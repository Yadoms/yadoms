#include "stdafx.h"
#include "ITask.h"
#include "Instance.h"
#include "IInstance.h"
#include "IUnique.h"
#include <shared/ThreadBase.h>
#include "TaskEvent.h"
#include "database/entities/Entities.h"
#include <shared/Log.h>

#include "Scheduler.h"
#include <Poco/UUIDGenerator.h>

#include <utility>

namespace task
{
   CScheduler::CScheduler(boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger)
      : CThreadBase("Task_Scheduler"),
        m_eventLogger(std::move(eventLogger)),
        m_taskEventHandler(boost::make_shared<shared::event::CEventHandler>())
   {
   }

   CScheduler::~CScheduler()
   {
      CThreadBase::stop();
   }

   void CScheduler::doWork()
   {
      YADOMS_LOG_CONFIGURE("taskScheduler")

      m_taskEventHandler->createTimer(kCleanFinishedTasks,
                                      shared::event::CEventTimer::kPeriodic,
                                      boost::posix_time::hours(12));

      while (!isStopping())
      {
         try
         {
            switch (m_taskEventHandler->waitForEvents())
            {
            case kTaskEvent:
               {
                  auto evt = m_taskEventHandler->getEventData<CTaskEvent>();
                  if (m_tasks.find(evt.getGuid()) == m_tasks.end())
                  {
                     //we have received an event from a task that is already finished
                     //nothing to do
                  }
                  else
                  {
                     auto task = m_tasks[evt.getGuid()];

                     if (task->getStatus() == ETaskStatus::kStartedValue)
                     {
                        database::entities::CEventLogger entry;
                        entry.Code = database::entities::ESystemEventCode::kStarted;
                        entry.Who = task->getTask()->getName();
                        m_eventLogger->addEvent(entry);
                     }
                  }
                  break;
               }
            case kCleanFinishedTasks:
               {
                  boost::lock_guard<boost::mutex> lock(m_mapsMutex);
                  if (m_tasks.empty())
                     break;

                  // Erase obsolete finished tasks
                  for (auto taskIterator = m_tasks.begin(); taskIterator != m_tasks.end();)
                  {
                     const auto task = taskIterator->second;

                     if (shared::currentTime::Provider().now() - task->getCreationDate() < boost::posix_time::hours(24))
                     {
                        ++taskIterator;
                        continue;
                     }

                     if (task->getStatus() == ETaskStatus::kStartedValue)
                     {
                        YADOMS_LOG(warning) << "Not finished task " << task->getName() << ", created at "
                           << to_simple_string(task->getCreationDate()) << ", stop it now...";

                        task->stop();
                     }
                     else
                     {
                        YADOMS_LOG(debug) << "Clean finished task " << task->getName() << ", created at "
                           << to_simple_string(task->getCreationDate());
                     }

                     taskIterator = m_tasks.erase(taskIterator);
                  }

                  YADOMS_LOG(information) << "Finished tasks cleaned";

                  break;
               }
            default:
               YADOMS_LOG(error) << "Unknown message id " << m_taskEventHandler->getEventId();
               BOOST_ASSERT(false);
               break;
            }
         }
         catch (boost::thread_interrupted&)
         {
            YADOMS_LOG(information) << "Thread is stopping, stop all tasks...";
            for (const auto& task : m_tasks)
               task.second->stop();
         }
         catch (const std::exception& e)
         {
            YADOMS_LOG(error) << "taskScheduler crashed in doWork with exception : " << e.what();

            database::entities::CEventLogger entry;
            entry.Code = database::entities::ESystemEventCode::kThreadFailed;
            entry.Who = "taskScheduler";
            entry.What = (boost::format("Crashed in doWork with exception %1%") % e.what()).str();
            m_eventLogger->addEvent(entry);
         }
         catch (...)
         {
            YADOMS_LOG(error) << "task scheduler unknown exception";

            database::entities::CEventLogger entry;
            entry.Code = database::entities::ESystemEventCode::kThreadFailed;
            entry.Who = "taskScheduler";
            entry.What = "crash with unknown exception";
            m_eventLogger->addEvent(entry);
         }
      }
   }

   bool CScheduler::runTask(const boost::shared_ptr<ITask>& taskToRun, std::string& uniqueId)
   {
      BOOST_ASSERT(taskToRun.get() != NULL);

      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      std::string existingUid;
      //we check if the task is IUnique
      if (boost::dynamic_pointer_cast<IUnique>(taskToRun) != nullptr)
      {
         //the task is unique, we must check if it is not already running

         const auto foundTask = std::find_if(m_tasks.begin(),
                                             m_tasks.end(),
                                             [&taskToRun](const auto& task)
                                             {
                                                return task.second->getName() == taskToRun->getName();
                                             });

         if (foundTask != m_tasks.end())
            existingUid = foundTask->first; // we have found the task that have already created
      }

      if (!existingUid.empty())
      {
         //the task can't be created because it is unique and it is already running
         //we return the uid of the older task
         uniqueId = existingUid;
         return false;
      }

      //the task is not unique or does not exist in the list, so we can create another
      uniqueId = Poco::UUIDGenerator::defaultGenerator().createRandom().toString();

      m_tasks[uniqueId] = boost::make_shared<CInstance>(taskToRun,
                                                        m_taskEventHandler,
                                                        kTaskEvent,
                                                        uniqueId);
      return true;
   }

   boost::shared_ptr<IInstance> CScheduler::getTask(const std::string& uniqueId)
   {
      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      if (m_tasks.find(uniqueId) != m_tasks.end())
         return m_tasks[uniqueId];

      return boost::shared_ptr<IInstance>();
   }

   std::vector<boost::shared_ptr<IInstance>> CScheduler::getTasks(const std::set<std::string>& ids)
   {
      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      std::vector<boost::shared_ptr<IInstance>> list;

      for (const auto& id : ids)
      {
         if (m_tasks.find(id) != m_tasks.end())
            list.emplace_back(m_tasks[id]);
      }

      return list;
   }

   std::vector<boost::shared_ptr<IInstance>> CScheduler::getAllTasks()
   {
      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      std::vector<boost::shared_ptr<IInstance>> list;

      std::transform(m_tasks.begin(),
                     m_tasks.end(),
                     back_inserter(list),
                     [](const auto& task)
                     {
                        return task.second;
                     });

      return list;
   }
} //namespace task
