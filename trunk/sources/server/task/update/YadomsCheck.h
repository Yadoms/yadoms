#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "IRunningInformation.h"
#include <shared/DataContainer.h>
#include <Poco/URI.h>

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadomsCheck : public ITask, public IUnique
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CYadomsCheck();

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CYadomsCheck();

   public:
      // ITask implementation
      virtual const std::string & getName();
      void doWork(TaskProgressFunc pFunctor);
      // ITask implementation
     
   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;
   };

} //namespace update
} //namespace task
