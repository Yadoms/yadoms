#include "stdafx.h"
#include "GenericTask.h"

namespace task
{
   CGenericTask::CGenericTask(const std::string& name, TaskFunc pFunctor)
      : m_taskName(name), m_pFunctor(pFunctor)
   {
   }

   CGenericTask::~CGenericTask()
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
} //namespace task


