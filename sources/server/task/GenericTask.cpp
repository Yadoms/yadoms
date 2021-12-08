#include "stdafx.h"
#include "GenericTask.h"

#include <utility>

namespace task
{
   CGenericTask::CGenericTask(std::string name,
                              TaskFunc pFunctor)
      : m_taskName(std::move(name)),
        m_pFunctor(std::move(pFunctor))
   {
   }

   const std::string& CGenericTask::getName() const
   {
      return m_taskName;
   }

   void CGenericTask::doWork(TaskProgressFunc pFunctor)
   {
      m_pFunctor(pFunctor);
   }

   void CGenericTask::onSetTaskId(const std::string& taskId)
   {
   }

   bool CGenericTask::isCancellable() const
   {
      return false;
   }
} //namespace task
