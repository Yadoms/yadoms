#pragma once
#include "IPythonExecutable.h"
#include <shared/process/IProcess.h>

class IFactory
{
public:
   virtual ~IFactory()
   {
   }

   virtual boost::shared_ptr<IPythonExecutable> createPythonExecutable() const = 0;
   virtual boost::shared_ptr<shared::process::IProcess> createScriptProcess(int scriptInstanceId,
                                                                            const std::string& scriptPath,
                                                                            boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                            const boost::filesystem::path& interpreterPath,
                                                                            const std::string& scriptApiId,
                                                                            boost::function2<void, bool, int> onInstanceStateChangedFct) const = 0;
};

