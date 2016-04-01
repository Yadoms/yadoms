#include "stdafx.h"
#include "Runner.h"
#include "ScriptFile.h"
#include "PythonException.hpp"
#include <shared/DataContainer.h>
#include "ContextAccessor.h"
#include "ScriptProcess.h"


CRunner::CRunner(const std::string& scriptPath,
   boost::shared_ptr<IPythonExecutable> executable,
   boost::shared_ptr<shared::process::ILogger> scriptLogger,
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
   boost::shared_ptr<shared::process::IStopNotifier> stopNotifier,
   const shared::CDataContainer& scriptConfiguration)
   :m_scriptFile(boost::make_shared<CScriptFile>(scriptPath)),
   m_executable(executable),
   m_scriptLogger(scriptLogger),
   m_scriptApi(yScriptApi),
   m_stopNotifier(stopNotifier),
   m_scriptConfiguration(scriptConfiguration)
{
   start();
}

CRunner::~CRunner()
{
   m_monitor.join();
}

void CRunner::start()
{
   m_scriptLogger->log("#### START ####");

   // Embed IYScriptApi instance
   m_contextAccessor = boost::make_shared<CContextAccessor>(m_scriptApi);

   try
   {
      m_process = boost::make_shared<CScriptProcess>(m_executable, m_scriptFile, m_contextAccessor->id(), m_scriptLogger);

      m_monitor = boost::thread(&CRunner::monitorThreaded, m_process, m_stopNotifier, m_scriptLogger);
   }
   catch(CPythonException& e)
   {
      m_scriptLogger->log((boost::format("%1% : error starting script, %2%") % m_scriptFile->pathName() % e.what()).str());
      m_stopNotifier->notifyStartError(e.what());
   }
}


void CRunner::requestStop()
{
   if (m_process)
      m_process->interrupt();
}

void CRunner::monitorThreaded(
   boost::shared_ptr<shared::process::IProcess> process,
   boost::shared_ptr<shared::process::IStopNotifier> stopNotifier,
   boost::shared_ptr<shared::process::ILogger> scriptLogger)
{
   if (process->waitForStop() == 0)
      stopNotifier->notifyNormalStop();
   else
      stopNotifier->notifyError(process->getError());
}