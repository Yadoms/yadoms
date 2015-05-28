#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update an existing plugin
   //-----------------------------------------
   class CPluginRemove : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] pluginName     The plugin name to remove
      //------------------------------------------
      CPluginRemove(const std::string & pluginName);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CPluginRemove();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual bool doWork(TaskProgressFunc pFunctor);
      // ITask implementation

   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;

      //------------------------------------------
      ///\brief   The plugin name
      //------------------------------------------
      std::string m_pluginName;
   };
} //namespace update
} //namespace task
