#include "stdafx.h"
#include "Factory.h"
#include "PythonExecutable.h"
#include "ProcessObserver.h"
#include <ScriptLogger.h>
#include "ScriptProcess.h"
#include "ScriptFile.h"


CFactory::CFactory()
{
}

CFactory::~CFactory()
{
}

boost::shared_ptr<IPythonExecutable> CFactory::createPythonExecutable() const
{
   return boost::make_shared<CPythonExecutable>();
}

boost::shared_ptr<shared::process::IProcessObserver> CFactory::createScriptProcessObserver(int scriptInstanceId,
                                                                                           boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct) const
{
   return boost::make_shared<CProcessObserver>(scriptInstanceId,
                                               onInstanceStateChangedFct);
}

boost::shared_ptr<shared::process::IExternalProcessLogger> CFactory::createScriptLogger(int scriptInstanceId,
                                                                                        const boost::filesystem::path& scriptLogPath) const
{
   return boost::make_shared<interpreter_cpp_api::CScriptLogger>(scriptInstanceId,
                                                                 scriptLogPath);
}

boost::shared_ptr<shared::process::IProcess> CFactory::createScriptProcess(int scriptInstanceId,
                                                                           const boost::filesystem::path& scriptPath,
                                                                           boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                           const boost::filesystem::path& interpreterPath,
                                                                           const std::string& scriptApiId,
                                                                           const boost::filesystem::path& scriptLogPath,
                                                                           boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct) const
{
   auto scriptLogger = createScriptLogger(scriptInstanceId,
                                          scriptLogPath);

   auto processObserver = createScriptProcessObserver(scriptInstanceId,
                                                      onInstanceStateChangedFct);

   return boost::make_shared<CScriptProcess>(pythonExecutable,
                                             interpreterPath,
                                             createScriptFile(scriptPath),
                                             scriptApiId,
                                             scriptLogger,
                                             processObserver);
}

boost::shared_ptr<IScriptFile> CFactory::createScriptFile(const boost::filesystem::path& scriptPath) const
{
   return boost::make_shared<CScriptFile>(scriptPath);
}
