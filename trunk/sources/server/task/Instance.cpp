#include "stdafx.h"
#include "ITask.h"
#include "Instance.h"
#include "TaskEvent.h"
#include <shared/Log.h>
#include "database/entities/Entities.h"
#include <shared/ServiceLocator.h>
#include "notification/Helpers.hpp"
#include <shared/StringExtension.h>
#include <shared/DataContainer.h>
#include "i18n/ClientStrings.h"

namespace task {

   //------------------------------
   ///\brief Constructor
   //------------------------------
   CInstance::CInstance(boost::shared_ptr<ITask> task, boost::shared_ptr<shared::event::CEventHandler> eventHandler, const int eventCode, const std::string & guid)
      : CThreadBase("Task " + task->getName()), m_currentProgression(0.0f), m_currentMessage(""), m_task(task), m_eventHandler(eventHandler), m_eventCode(eventCode), 
      m_guid(guid), m_currentStatus(ETaskStatus::kStarted), 
      m_creationDate(boost::posix_time::second_clock::universal_time())
   {
      BOOST_ASSERT(m_task);
      start();
   }

   //------------------------------
   ///\brief public destructor
   //------------------------------
   CInstance::~CInstance()
   {
      stop();
   }
   
   boost::shared_ptr<ITask> CInstance::getTask()
   {
      return m_task;
   }

   std::string CInstance::getGuid() const
   {
      return m_guid;
   }

   boost::optional<float> CInstance::getProgression() const
   {
      return m_currentProgression;
   }

   std::string CInstance::getMessage() const
   {
      return m_currentMessage;
   }

   ETaskStatus CInstance::getStatus() const
   {
      return m_currentStatus;
   }

   std::string CInstance::getName() const
   {
      return m_task->getName();
   }

   shared::CDataContainer CInstance::getTaskData() const
   {
      return m_taskData;
   }

   boost::posix_time::ptime CInstance::getCreationDate() const
   {
      return m_creationDate;
   }

   void CInstance::OnTaskProgressUpdated(bool isRunning, boost::optional<float> progression, std::string message, shared::CDataContainer data)
   {
      m_currentProgression = progression;
      m_currentMessage = message;
      m_taskData = data;
      if (m_currentProgression)
         YADOMS_LOG(information) << m_task->getName() << " report progression " << m_currentProgression.get() << " with message " << m_currentMessage;
      else
         YADOMS_LOG(information) << m_task->getName() << " report progression none with message " << m_currentMessage;

      // Post notification
      boost::shared_ptr<IInstance> obj(shared_from_this());
      notification::CHelpers::postNotification(obj);
   }

   void CInstance::doWork()
   {
      BOOST_ASSERT(m_task);
      YADOMS_LOG_CONFIGURE(m_task->getName());

      try
      {
         //task event started
         m_currentStatus = ETaskStatus::kStarted;
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));

         OnTaskProgressUpdated(true, 0.0f, i18n::CClientStrings::TaskStarted, shared::CDataContainer::EmptyContainer);

         // Execute task code
         if (m_task->doWork(boost::bind(&CInstance::OnTaskProgressUpdated, this, _1, _2, _3, _4)))
         {
            m_currentStatus = ETaskStatus::kSuccess;
         }
         else
         {
            m_currentStatus = ETaskStatus::kFail;
         }
         
         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskEnd, shared::CDataContainer::EmptyContainer);

         //task event stopped
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
      catch (boost::thread_interrupted&)
      {
         // End-of-thread exception was not catch by task,
         // it's a developer's error, we have to report it
         YADOMS_LOG(error) << m_task->getName() << " didn't catch boost::thread_interrupted.";
         m_currentStatus = ETaskStatus::kFail;
         m_currentMessage = "thread_interrupted not catched";

         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskInterrupted, shared::CDataContainer::EmptyContainer);

         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
      catch (std::exception& e)
      {
         // Task crashed
         YADOMS_LOG(error) << m_task->getName() << " crashed in doWork with exception : " << e.what();
         m_currentStatus = ETaskStatus::kFail;
         m_currentMessage = e.what();

         shared::CDataContainer data;
         data.set("exception", e.what());
         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskCrashed, data);

         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
      catch (...)
      {
         // Plugin crashed
         YADOMS_LOG(error) << m_task->getName() << " crashed in doWork with unknown exception.";
         m_currentStatus = ETaskStatus::kFail;
         m_currentMessage = "unknown exception not catched";
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));

         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskUnkownError, shared::CDataContainer::EmptyContainer);
      }
   }
   
} //namespace task