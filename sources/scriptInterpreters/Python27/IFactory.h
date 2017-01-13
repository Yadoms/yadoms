#pragma once
#include "IPythonExecutable.h"
#include <shared/process/IProcess.h>
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>


// Shortcut to yInterpreterApi namespace
namespace yApi = shared::script::yInterpreterApi;

class IFactory
{
public:
   virtual ~IFactory()
   {
   }

   virtual boost::shared_ptr<IPythonExecutable> createPythonExecutable() const = 0;
   virtual boost::shared_ptr<shared::process::IProcess> createScriptProcess(boost::shared_ptr<yApi::IYInterpreterApi> api,
                                                                            int scriptInstanceId,
                                                                            const std::string& scriptPath,
                                                                            boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                            const boost::filesystem::path& interpreterPath,
                                                                            const std::string& scriptApiId,
                                                                            boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct) const = 0;
};

