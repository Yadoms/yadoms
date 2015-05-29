#pragma once
#include "task/ITask.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   ScriptInterpreter install task. The aim si to install a new scriptInterpreter
   //-----------------------------------------
   class CScriptInterpreterInstall : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] downloadUrl    The download url
      //------------------------------------------
      CScriptInterpreterInstall(const std::string & downloadUrl);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CScriptInterpreterInstall();

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
      ///\brief   The download url
      //------------------------------------------
      std::string m_downloadUrl;
   };

} //namespace update
} //namespace task
