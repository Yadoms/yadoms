#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   ScriptInterpreter update task. The aim si to update an existing scriptInterpreter
   //-----------------------------------------
   class CScriptInterpreterUpdate : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] scriptInterpreterName     The scriptInterpreter name to update
      ///\param [in] downloadUrl    The download url
      //------------------------------------------
      CScriptInterpreterUpdate(const std::string & scriptInterpreterName, const std::string & downloadUrl);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CScriptInterpreterUpdate();

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

      //------------------------------------------
      ///\brief   The scriptInterpreter name
      //------------------------------------------
      std::string m_scriptInterpreterName;

      //------------------------------------------
      ///\brief   The download url
      //------------------------------------------
      std::string m_downloadUrl;
   };


} //namespace update
} //namespace task
