#include "stdafx.h"
#include "Runner.h"
#include "ScriptFile.h"
#include "PythonException.hpp"
#include "ContextAccessor.h"
#include <shared/DataContainer.h>


CRunner::CRunner(const std::string& scriptPath, const std::string& interpreterPath,
   boost::shared_ptr<IPythonExecutable> executable, const shared::CDataContainer& scriptConfiguration)
   :m_scriptFile(boost::make_shared<CScriptFile>(scriptPath)), m_scriptConfiguration(scriptConfiguration), m_interpreterPath(interpreterPath), m_executable(executable)
{
}

CRunner::~CRunner()
{
}

void CRunner::run(shared::script::yScriptApi::IYScriptApi& context, boost::shared_ptr<shared::script::ILogger> scriptLogger)
{
   m_lastError.clear();
   scriptLogger->log("#### START ####");

   // Embed IYScriptApi instance
   boost::shared_ptr<IContextAccessor> contextAccessor(boost::make_shared<CContextAccessor>(context));

   try
   {
      m_executable->startModule(m_scriptFile, contextAccessor->id(), scriptLogger);
      m_executable->waitForStop();
   }
   catch(CPythonException& e)
   {
      scriptLogger->log((boost::format("%1% : error running script, %2%") % m_scriptFile->pathName() % e.what()).str());
      m_lastError = e.what();
   }

   scriptLogger->log("#### END ####");
}


void CRunner::interrupt()
{
   m_executable->interrupt();
}

bool CRunner::isOk() const
{
   return m_lastError.empty();
}

std::string CRunner::error() const
{
   return m_lastError;
}
