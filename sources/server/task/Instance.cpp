#include "stdafx.h"
#include "ITask.h"
#include "Instance.h"
#include "TaskEvent.h"
#include <shared/currentTime/Provider.h>
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include "notification/Helpers.hpp"
#include <shared/DataContainer.h>

#include <utility>
#include "i18n/ClientStrings.h"

#include "InstanceNotificationData.h"

namespace task
{
   CInstance::CInstance(const boost::shared_ptr<ITask>& task,
                        boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                        const int eventCode,
                        std::string guid)
      : CThreadBase("Task " + task->getName()),
        m_currentIsRunning(false),
        m_currentProgression(0.0f),
        m_currentMessage(""),
        m_taskData(shared::CDataContainer::make()),
        m_task(task),
        m_eventHandler(std::move(eventHandler)),
        m_eventCode(eventCode),
        m_guid(std::move(guid)),
        m_currentStatus(ETaskStatus::kStarted),
        m_creationDate(shared::currentTime::Provider().now())
   {
      BOOST_ASSERT(m_task);
      CThreadBase::start();
   }

   CInstance::~CInstance()
   {
      CThreadBase::stop();
   }

   boost::shared_ptr<ITask> CInstance::getTask() const
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

   std::string CInstance::getExceptionMessage() const
   {
      return m_currentException;
   }

   ETaskStatus CInstance::getStatus() const
   {
      return m_currentStatus;
   }

   std::string CInstance::getName() const
   {
      return m_task->getName();
   }

   boost::shared_ptr<shared::CDataContainer> CInstance::getTaskData() const
   {
      return m_taskData;
   }

   boost::posix_time::ptime CInstance::getCreationDate() const
   {
      return m_creationDate;
   }

   void CInstance::OnTaskProgressUpdated(bool isRunning, boost::optional<float> progression, const std::string& message, const std::string& exception,
                                         boost::shared_ptr<shared::CDataContainer> taskData)
   {
      if (!isRunning)
         m_currentStatus = ETaskStatus::kFail;
      else
      {
         if (progression)
         {
            if (progression >= 100.0f)
               m_currentStatus = ETaskStatus::kSuccess;
         }
      }
      m_currentIsRunning = isRunning;
      m_currentProgression = progression;
      m_currentMessage = message;
      m_currentException = exception;
      m_taskData = std::move(taskData);

      if (m_currentProgression)
         YADOMS_LOG(debug) << m_task->getName() << " report progression " << m_currentProgression.get() << " with message " << m_currentMessage;
      else
         YADOMS_LOG(debug) << m_task->getName() << " report progression none with message " << m_currentMessage;

      // Post notification
      const auto obj = boost::make_shared<CInstanceNotificationData>(*this);
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

         OnTaskProgressUpdated(true, 0.0f, i18n::CClientStrings::TaskStarted, std::string(), shared::CDataContainer::make());

         // Execute task code
         m_task->onSetTaskId(getGuid());
         m_task->doWork(boost::bind(&CInstance::OnTaskProgressUpdated, this, boost::placeholders::_1, boost::placeholders::_2,
                                    boost::placeholders::_3, boost::placeholders::_4, boost::placeholders::_5));

         //check if task is still running (modified by callback)
         if (m_currentIsRunning && m_currentStatus != ETaskStatus::kSuccess)
         {
            m_currentStatus = ETaskStatus::kSuccess;
            OnTaskProgressUpdated(true, 100.0, i18n::CClientStrings::TaskEnd, std::string(), shared::CDataContainer::make());
         }

         //task event stopped
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
      catch (boost::thread_interrupted&)
      {
         // End-of-thread exception was not catch by task,
         // it's a developer's error, we have to report it
         YADOMS_LOG(error) << m_task->getName() << " didn't catch boost::thread_interrupted.";
         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskInterrupted, "Task interrupted.", shared::CDataContainer::make());
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
      catch (std::exception& e)
      {
         // Task crashed
         YADOMS_LOG(error) << m_task->getName() << " crashed in doWork with exception : " << e.what();
         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskCrashed, e.what(), shared::CDataContainer::make());
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
      catch (...)
      {
         // Plugin crashed
         YADOMS_LOG(error) << m_task->getName() << " crashed in doWork with unknown exception.";
         OnTaskProgressUpdated(false, 100.0, i18n::CClientStrings::TaskUnkownError, "unknown exception not catched", shared::CDataContainer::make());
         m_eventHandler->postEvent(m_eventCode, CTaskEvent(m_guid));
      }
   }
} //namespace task
