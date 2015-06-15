#include "stdafx.h"
#include "ScriptInterpreterRemove.h"
#include <shared/Log.h>

#include "update/worker/ScriptInterpreter.h"

namespace task { namespace update {

   std::string CScriptInterpreterRemove::m_taskName = "scriptInterpreter.remove";

   CScriptInterpreterRemove::CScriptInterpreterRemove(const std::string & scriptInterpreterName)
      :m_scriptInterpreterName(scriptInterpreterName)
   {
   }

   CScriptInterpreterRemove::~CScriptInterpreterRemove()
   {
   }

   const std::string & CScriptInterpreterRemove::getName()
   {
      return m_taskName;
   }

   void CScriptInterpreterRemove::doWork(TaskProgressFunc pFunctor)
   {
      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CScriptInterpreter worker(boost::bind(pFunctor, _1, _2, _3, _4, _5));
      worker.remove(m_scriptInterpreterName);
   }

} //namespace update
} //namespace task
