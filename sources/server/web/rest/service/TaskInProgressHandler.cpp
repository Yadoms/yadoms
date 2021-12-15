#include "stdafx.h"
#include "TaskInProgressHandler.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CTaskInProgressHandler::CTaskInProgressHandler(boost::shared_ptr<task::CScheduler> taskScheduler)
            : m_taskScheduler(std::move(taskScheduler))
         {
         }

         std::string CTaskInProgressHandler::inProgressTaskUid()
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_inProgressTaskUidMutex);

            if (m_inProgressTaskUid.empty())
               return std::string();

            if (m_taskScheduler->getTask(m_inProgressTaskUid)->getStatus() != task::ETaskStatus::kStarted)
               m_inProgressTaskUid.clear();

            return m_inProgressTaskUid;
         }

         void CTaskInProgressHandler::setInProgressTaskUid(const std::string& taskUid)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_inProgressTaskUidMutex);
            m_inProgressTaskUid = taskUid;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
