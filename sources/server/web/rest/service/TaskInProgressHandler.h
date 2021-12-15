#pragma once
#include "ITaskInProgressHandler.h"
#include "task/Scheduler.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CTaskInProgressHandler final : public ITaskInProgressHandler
         {
         public:
            explicit CTaskInProgressHandler(boost::shared_ptr<task::CScheduler> taskScheduler);

            ~CTaskInProgressHandler() override = default;

            // ITaskInProgressHandler Implementation
            std::string inProgressTaskUid() override;
            void setInProgressTaskUid(const std::string& taskUid) override;
            // [END] ITaskInProgressHandler Implementation

         private:
            const boost::shared_ptr<task::CScheduler> m_taskScheduler;
            mutable boost::recursive_mutex m_inProgressTaskUidMutex;
            std::string m_inProgressTaskUid;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
