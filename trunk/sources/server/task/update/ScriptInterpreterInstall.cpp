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

   bool CScriptInterpreterInstall::doWork(TaskProgressFunc pFunctor)
   {

      YADOMS_LOG(information) << "Start installing scriptInterpreter...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CScriptInterpreter::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3, _4);

      ::update::worker::CScriptInterpreter worker(adapter);
      worker.install(m_downloadUrl);

      YADOMS_LOG(information) << "End of installing scriptInterpreter";
      return true;
   }

} //namespace update
} //namespace task
