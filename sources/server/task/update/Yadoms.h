#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadoms : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CYadoms();

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CYadoms();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual void doWork(TaskProgressFunc pFunctor);
      // ITask implementation

   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;
   };

} //namespace update
} //namespace task
