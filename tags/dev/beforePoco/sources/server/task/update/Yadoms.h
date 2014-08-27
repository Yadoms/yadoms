#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadoms : public ITask, public IUnique
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
      bool doWork(TaskProgressFunc pFunctor);
      // ITask implementation

      void onDownloadReportProgress(const std::string & filename, float progression);

   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;
   };

} //namespace update
} //namespace task
