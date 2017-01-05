#pragma once
#include "IPythonExecutable.h"
#include <shared/process/IProcessObserver.h>
#include <shared/process/IExternalProcessLogger.h>
#include <shared/process/IProcess.h>
#include <shared/process/ICommandLine.h>

class IFactory
{
public:
   virtual ~IFactory()
   {
   }

   virtual boost::shared_ptr<IPythonExecutable> createPythonExecutable() const = 0;
   virtual boost::shared_ptr<shared::process::IProcessObserver> createScriptProcessObserver(int scriptInstanceId,
                                                                                            boost::function2<void, bool, int> onInstanceStateChangedFct) const = 0;
   virtual boost::shared_ptr<shared::process::IExternalProcessLogger> createScriptLogger(int scriptInstanceId) const = 0;
   virtual boost::shared_ptr<shared::process::IProcess> createScriptProcess(const std::string& scriptPath,
                                                                            boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                            const boost::filesystem::path& interpreterPath,
                                                                            boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                                                                            const std::string& scriptApiId,
                                                                            boost::shared_ptr<shared::process::IProcessObserver> processObserver) const = 0;
};

