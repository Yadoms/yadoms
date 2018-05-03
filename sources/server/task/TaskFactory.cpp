#include "stdafx.h"
#include "ITask.h"
#include "TaskFactory.h"
#include <shared/exception/Exception.hpp>

namespace task { 

   CTaskFactory::CTaskFactory()
   {
   }

   CTaskFactory::~CTaskFactory()
   {
   }

   boost::shared_ptr<ITask> CTaskFactory::createTask(const std::string & type)
   {
      //if (boost::iequals(type, "system.yadomsUpdate"))
      //   return boost::shared_ptr<ITask>(new update::CYadoms());
      
      throw shared::exception::CException("Can't create a task using type: " + type);
      //TODO : create other task creation
   }
} //namespace task
