#include "stdafx.h"
#include "GenericTask.h"

#include <utility>

namespace task
{
   CGenericTask::CGenericTask(std::string name,
                              std::function<void(TaskProgressFunc)> taskFct)
      : m_taskName(std::move(name)),
        m_taskFct(std::move(taskFct))
   {
   }

   std::string CGenericTask::getName()
   {
      return m_taskName;
   }

   void CGenericTask::doWork(TaskProgressFunc reportProgressFct)
   {
      m_taskFct(reportProgressFct);
   }

   void CGenericTask::onSetTaskId(const std::string& taskId)
   {
   }

   bool CGenericTask::isCancellable() const
   {
      return false;
   }
} //namespace task
