#pragma once
#include <string>

namespace task
{
   //----------------------------
   ///\brief   Interface for unique long task handling
   //----------------------------
   class IRunningTaskMutex
   {
   public:
      virtual ~IRunningTaskMutex() = default;

      virtual std::string inProgressTaskUid() = 0;
      virtual void setInProgressTaskUid(const std::string& taskUid) = 0;
   };
} //namespace task 
