#include "stdafx.h"
#include "RunningTaskMutex.h"

namespace task
{
   CRunningTaskMutex::CRunningTaskMutex(boost::shared_ptr<task::CScheduler> taskScheduler)
      : m_taskScheduler(std::move(taskScheduler))
   {
   }

   std::string CRunningTaskMutex::inProgressTaskUid()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_inProgressTaskUidMutex);

      if (m_inProgressTaskUid.empty())
         return std::string();

      if (m_taskScheduler->getTask(m_inProgressTaskUid)->getStatus() != task::ETaskStatus::kStarted)
         m_inProgressTaskUid.clear();

      return m_inProgressTaskUid;
   }

   void CRunningTaskMutex::setInProgressTaskUid(const std::string& taskUid)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_inProgressTaskUidMutex);
      m_inProgressTaskUid = taskUid;
   }
} //namespace task 
