#include "stdafx.h"
#include "ITask.h"
#include "Instance.h"
#include "IInstance.h"
#include "IUnique.h"
#include "shared/ThreadBase.h"
#include "TaskEvent.h"
#include "database/entities/Entities.h"
#include <shared/Log.h>

#include "Scheduler.h"

namespace task {

   CScheduler::CScheduler(boost::shared_ptr<shared::event::CEventHandler> eventHandler, const int systemEventCode)
      : CThreadBase("Task_Scheduler"), m_eventHandler(eventHandler), m_taskEventHandler(new shared::event::CEventHandler), m_systemEventCode(systemEventCode)
   {
   }

   CScheduler::~CScheduler()
   {
      stop();
   }

   void CScheduler::doWork()
   {
      YADOMS_LOG_CONFIGURE("taskScheduler");
      while (!isStopping())
      {
         try
         {
            switch (m_taskEventHandler->waitForEvents())
            {
            case kTaskEvent:
            {
               CTaskEvent evt = m_taskEventHandler->getEventData<CTaskEvent>();
               if (m_runningTasks.find(evt.getGuid()) == m_runningTasks.end())
               {
                  //we have received an event from a task that is already finished
                  //nothing to do
               }
               else
               {
                  boost::shared_ptr<CInstance> i = m_runningTasks[evt.getGuid()];

                  switch (i->getStatus())
                  {
                  case ITask::kStarted:
                  {
                     //the task has started
                     database::entities::CEventLogger entry;
                     entry.Code = database::entities::ESystemEventCode::kStarted;
                     entry.Who = m_runningTasks[i->getGuid()]->getTask()->getName();
                     m_eventHandler->postEvent(m_systemEventCode, entry);
                     break;
                  }

                  case ITask::kSuccess:
                  case ITask::kFail:
                  {
                     //we need to take the mutex
                     boost::lock_guard<boost::mutex> lock(m_mapsMutex);

                     //the task has finished
                     //we create a finishedInstance container
                     m_finishedTasks.insert(std::make_pair(i->getGuid(), boost::shared_ptr<CFinishedInstance>(new CFinishedInstance(i->getGuid(), i->getName(), i->getStatus(), i->getProgression(), i->getMessage(), i->getCreationDate()))));

                     //we remove the actual instance from the dictionnary
                     m_runningTasks.erase(i->getGuid());
                     //we destroy the evt object
                     i.reset();
                     break;
                  }

                  default:
                     //nothing to do
                     break;

                  }
               }
               break;
            }
            default:
               YADOMS_LOG(error) << "Unknown message id";
               BOOST_ASSERT(false);
               break;
            }

         }
         catch (boost::thread_interrupted&)
         {
            YADOMS_LOG(info) << "Thread is stopping, stop all tasks...";
            RunningTaskInstanceMap::iterator it;
            for (it = m_runningTasks.begin(); it != m_runningTasks.end(); ++it)
               it->second->stop();
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "taskScheduler crashed in doWork with exception : " << e.what();

            database::entities::CEventLogger entry;
            entry.Code = database::entities::ESystemEventCode::kThreadFailed;
            entry.Who = "taskScheduler";
            entry.What = (boost::format("Crashed in doWork with exception %1%") % e.what()).str();
            m_eventHandler->postEvent(m_systemEventCode, entry);
         }
         catch (...)
         {
            YADOMS_LOG(error) << "task scheduler unkonwn exception";

            database::entities::CEventLogger entry;
            entry.Code = database::entities::ESystemEventCode::kThreadFailed;
            entry.Who = "taskScheduler";
            entry.What = "crash with unknown exception.";
            m_eventHandler->postEvent(m_systemEventCode, entry);
         }
      }
   }

   bool CScheduler::runTask(boost::shared_ptr<ITask> taskToRun, std::string & uniqueId)
   {
      BOOST_ASSERT(taskToRun.get() != NULL);

      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      std::string existingUid = "";
      //we check if the task is IUnique
      if (boost::dynamic_pointer_cast<IUnique>(taskToRun) != NULL)
      {
         //the task is unique, we must check if it is not already running

         RunningTaskInstanceMap::iterator it;
         for (it = m_runningTasks.begin(); it != m_runningTasks.end(); ++it)
         {
            if (boost::iequals(it->second->getName(), taskToRun->getName()))
            {
               //we have found the task that have already created
               existingUid = it->first;
            }
         }
      }

      if (existingUid != "")
      {
         //the task can't be created because it is unique and it is already running
         //we return the uid of the older task
         uniqueId = existingUid;
         return false;
      }

      //the task is not unique or does not exist in the list, so we can create another
      boost::uuids::uuid u = boost::uuids::random_generator()();
      uniqueId = boost::uuids::to_string(u);
      
      m_runningTasks.insert(std::make_pair(uniqueId, boost::shared_ptr<CInstance>(new CInstance(taskToRun, m_taskEventHandler, kTaskEvent, uniqueId))));
      return true;
   }
   
   boost::shared_ptr<IInstance> CScheduler::getTask(std::string uniqueId)
   {
      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      if (m_runningTasks.find(uniqueId) != m_runningTasks.end())
         return m_runningTasks[uniqueId];
      if (m_finishedTasks.find(uniqueId) != m_finishedTasks.end())
         return m_finishedTasks[uniqueId];
      return boost::shared_ptr<IInstance>();
   }

   std::vector< boost::shared_ptr< IInstance > > CScheduler::getAllTasks()
   {
      boost::lock_guard<boost::mutex> lock(m_mapsMutex);

      std::vector< boost::shared_ptr< IInstance > > list;

      RunningTaskInstanceMap::iterator it;
      for (it = m_runningTasks.begin(); it != m_runningTasks.end(); ++it)
      {
         list.push_back(it->second);
      }

      FinishedTaskInstanceMap::iterator it2;
      for (it2 = m_finishedTasks.begin(); it2 != m_finishedTasks.end(); ++it2)
      {
         list.push_back(it2->second);
      }

      return list;
   }

} //namespace task

