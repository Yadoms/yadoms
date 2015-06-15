#include "stdafx.h"
#include "ScriptInterpreterInstall.h"
#include <shared/Log.h>
#include "update/worker/ScriptInterpreter.h"

namespace task { namespace update {

   std::string CScriptInterpreterInstall::m_taskName = "scriptInterpreter.install";

   CScriptInterpreterInstall::CScriptInterpreterInstall(const std::string & downloadUrl)
      :m_downloadUrl(downloadUrl)
   {
   }

   CScriptInterpreterInstall::~CScriptInterpreterInstall()
   {
   }

   const std::string & CScriptInterpreterInstall::getName()
   {
      return m_taskName;
   }

   void CScriptInterpreterInstall::doWork(TaskProgressFunc pFunctor)
   {
      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CScriptInterpreter worker(boost::bind(pFunctor, _1, _2, _3, _4, _5));
      worker.install(m_downloadUrl);
   }

} //namespace update
} //namespace task
