#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   ScriptInterpreter update task. The aim si to update an existing scriptInterpreter
   //-----------------------------------------
   class CScriptInterpreterRemove : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] scriptInterpreterName     The scriptInterpreter name to remove
      //------------------------------------------
      CScriptInterpreterRemove(const std::string & scriptInterpreterName);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CScriptInterpreterRemove();

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
      ///\brief   The scriptInterpreter name
      //------------------------------------------
      std::string m_scriptInterpreterName;
   };
} //namespace update
} //namespace task
