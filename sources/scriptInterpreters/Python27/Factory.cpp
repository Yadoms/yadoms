#include "stdafx.h"
#include "Factory.h"
#include "PythonExecutable.h"
#include "ProcessObserver.h"
#include "ScriptLogger.h"
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
                                                                                           boost::function2<void, bool, int> onInstanceStateChangedFct) const
{
   return boost::make_shared<CProcessObserver>(scriptInstanceId,
                                               onInstanceStateChangedFct);
}

boost::shared_ptr<shared::process::IExternalProcessLogger> CFactory::createScriptLogger(int scriptInstanceId) const
{
   return boost::make_shared<CScriptLogger>(scriptInstanceId);
}

boost::shared_ptr<shared::process::IProcess> CFactory::createScriptProcess(const std::string& scriptPath,
                                                                           boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                           const boost::filesystem::path& interpreterPath,
                                                                           boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                                                                           const std::string& scriptApiId,
                                                                           boost::shared_ptr<shared::process::IProcessObserver> processObserver) const
{
   return boost::make_shared<CScriptProcess>(pythonExecutable,
                                             interpreterPath,
                                             createScriptFile(scriptPath),
                                             scriptApiId,
                                             scriptLogger,
                                             processObserver);
}

boost::shared_ptr<IScriptFile> CFactory::createScriptFile(const std::string& scriptPath) const
{
   return boost::make_shared<CScriptFile>(scriptPath);
}

