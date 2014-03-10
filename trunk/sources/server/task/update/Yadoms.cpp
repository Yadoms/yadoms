#include "stdafx.h"
#include "Yadoms.h"

namespace task { namespace update {

   std::string CYadoms::m_taskName = "update yadoms task";

   CYadoms::CYadoms()
   {
   }

   CYadoms::~CYadoms()
   {
   }

   const std::string & CYadoms::getName()
   {
      return m_taskName;
   }

   void CYadoms::doWork(TaskProgressFunc pFunctor)
   {
   }

} //namespace update
} //namespace task
