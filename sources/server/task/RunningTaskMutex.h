#pragma once
#include "IRunningTaskMutex.h"
#include "Scheduler.h"

namespace task
{
   class CRunningTaskMutex final : public IRunningTaskMutex
   {
   public:
      explicit CRunningTaskMutex(boost::shared_ptr<CScheduler> taskScheduler);

      ~CRunningTaskMutex() override = default;

      // ITaskInProgressHandler Implementation
      std::string inProgressTaskUid() override;
      void setInProgressTaskUid(const std::string& taskUid) override;
      // [END] ITaskInProgressHandler Implementation

   private:
      const boost::shared_ptr<task::CScheduler> m_taskScheduler;
      mutable boost::recursive_mutex m_inProgressTaskUidMutex;
      std::string m_inProgressTaskUid;
   };
} //namespace task 
