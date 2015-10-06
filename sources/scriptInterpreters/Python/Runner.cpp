#include "stdafx.h"
#include "Runner.h"
#include "ScriptFile.h"
#include "PythonException.hpp"
#include <shared/DataContainer.h>
#include "ContextAccessor.h"


CRunner::CRunner(const std::string& scriptPath,
   boost::shared_ptr<IPythonExecutable> executable,
   boost::shared_ptr<shared::script::ILogger> scriptLogger,
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
   boost::shared_ptr<shared::script::IStopNotifier> stopNotifier,
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
   stop();
}

void CRunner::start()
{
   m_lastError.clear();
   m_scriptLogger->log("#### START ####");

   // Embed IYScriptApi instance
   m_contextAccessor = boost::make_shared<CContextAccessor>(m_scriptApi);

   try
   {
      m_executable->startModule(m_scriptFile, m_contextAccessor->id(), m_scriptLogger);
   }
   catch(CPythonException& e)
   {
      m_scriptLogger->log((boost::format("%1% : error starting script, %2%") % m_scriptFile->pathName() % e.what()).str());
      m_lastError = e.what();
      m_stopNotifier->notifyStartError(m_lastError);
      m_scriptLogger->log("#### END ####");
   }

   m_monitor = boost::thread(&CRunner::monitor, this, m_stopNotifier);
}


void CRunner::stop()
{
   if (m_executable)
      m_executable->interrupt();

   m_monitor.join();
}

bool CRunner::isOk() const
{
   return m_lastError.empty();
}

std::string CRunner::error() const
{
   return m_lastError;
}

void CRunner::monitor(boost::shared_ptr<shared::script::IStopNotifier> stopNotifier)
{
   if (m_executable->waitForStop() == 0)
   {
      m_lastError.clear();
      stopNotifier->notifyNormalStop();
   }
   else
   {
      m_lastError = "Error"; // TODO récupérer l'erreur Python
      stopNotifier->notifyError(m_lastError);
   }

   m_scriptLogger->log("#### END ####");
}