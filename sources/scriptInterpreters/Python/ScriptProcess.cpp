#include "stdafx.h"
#include "ScriptProcess.h"
#include "PythonCommandLine.h"
#include <shared/process/Process.h>
#include <shared/FileSystemExtension.h>


CScriptProcess::CScriptProcess(boost::shared_ptr<IPythonExecutable> executable,
                               const boost::filesystem::path& pythonInterpreterPath,
                               boost::shared_ptr<const IScriptFile> scriptFile,
                               boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                               const std::string &scriptLogger,
                               boost::shared_ptr<shared::process::IProcessObserver> stopNotifier)
   : m_executable(executable),
     m_pythonInterpreterPath(pythonInterpreterPath),
     m_scriptFile(scriptFile),
     m_yScriptApi(yScriptApi),
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
                                                 m_pythonInterpreterPath.parent_path(),
                                                 args);
}

void CScriptProcess::start()
{
   m_contextAccessor = boost::make_shared<CContextAccessor>(m_yScriptApi);

   auto commandLine = createCommandLine(m_contextAccessor->id());

   m_process = boost::make_shared<shared::process::CProcess>(commandLine,
                                                             shared::CFileSystemExtension::getModulePath().string(),
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
