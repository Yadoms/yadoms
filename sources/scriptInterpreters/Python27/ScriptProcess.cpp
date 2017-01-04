#include "stdafx.h"
#include "ScriptProcess.h"
#include "PythonCommandLine.h"
#include <shared/process/ProcessDeprecated.h>


CScriptProcess::CScriptProcess(boost::shared_ptr<IPythonExecutable> executable,
                               const boost::filesystem::path& interpreterPath,
                               boost::shared_ptr<const IScriptFile> scriptFile,
                               const std::string& scriptApiId,
                               boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                               boost::shared_ptr<shared::process::IProcessObserver> stopNotifier)
   : m_executable(executable),
     m_interpreterPath(interpreterPath),
     m_scriptFile(scriptFile),
     m_scriptApiId(scriptApiId),
     m_scriptLogger(scriptLogger),
     m_stopNotifier(stopNotifier)
{
   start();
}

CScriptProcess::~CScriptProcess()
{
   CScriptProcess::kill();
}

boost::shared_ptr<shared::process::ICommandLine> CScriptProcess::createCommandLine(const std::string& apiIdentifier) const
{
   std::vector<std::string> args;
   args.push_back("-u"); // Make script outs unbuffered
   args.push_back("scriptCaller.py");
   args.push_back(m_scriptFile->abslouteParentPath().string());
   args.push_back(m_scriptFile->module());
   args.push_back(apiIdentifier);

   return boost::make_shared<CPythonCommandLine>(m_executable->path(),
                                                 m_interpreterPath,
                                                 args);
}

void CScriptProcess::start()
{
   auto commandLine = createCommandLine(m_scriptApiId);

   m_process = boost::make_shared<shared::process::CProcessDeprecated>(commandLine,
                                                                       m_stopNotifier,
                                                                       m_scriptLogger);
}

void CScriptProcess::kill()
{
   m_process->kill();
}

int CScriptProcess::getReturnCode() const
{
   return m_process->getReturnCode();
}

std::string CScriptProcess::getError() const
{
   return m_process->getError();
}

