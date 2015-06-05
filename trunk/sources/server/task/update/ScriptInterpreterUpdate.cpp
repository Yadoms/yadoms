#include "stdafx.h"
#include "ScriptInterpreterUpdate.h"
#include <shared/Log.h>
#include "update/worker/ScriptInterpreter.h"

namespace task { namespace update {

   std::string CScriptInterpreterUpdate::m_taskName = "scriptInterpreter.update";

   CScriptInterpreterUpdate::CScriptInterpreterUpdate(const std::string & scriptInterpreterName, const std::string & downloadUrl)
      :m_scriptInterpreterName(scriptInterpreterName), m_downloadUrl(downloadUrl)
   {
   }

   CScriptInterpreterUpdate::~CScriptInterpreterUpdate()
   {
   }

   const std::string & CScriptInterpreterUpdate::getName()
   {
      return m_taskName;
   }

   bool CScriptInterpreterUpdate::doWork(TaskProgressFunc pFunctor)
   {
      YADOMS_LOG(information) << "Start updating scriptInterpreter...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CScriptInterpreter worker(boost::bind(pFunctor, _1, _2, _3, _4));
      worker.update(m_scriptInterpreterName, m_downloadUrl);

      YADOMS_LOG(information) << "End of updating scriptInterpreter";
      return true;
   }

} //namespace update
} //namespace task
