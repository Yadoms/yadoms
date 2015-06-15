#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"
#include <shared/DataContainer.h>

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadomsUpdate : public ITask, public IUnique
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] updateSource   The update source
      ///\param [in] onlyCheckForUpdate   If true will perform a check for update; if false will perform a complete update
      //------------------------------------------
      CYadomsUpdate(const shared::CDataContainer & versionToUpdate);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CYadomsUpdate();

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

      //------------------------------------------
      ///\brief   The versionToUpdate.json content
      //------------------------------------------
      shared::CDataContainer m_versionToUpdate;
   };

} //namespace update
} //namespace task
