#pragma once
#include "IFactory.h"
#include "IScriptFile.h"
#include <shared/process/IProcessObserver.h>

//--------------------------------------------------------------
/// \brief	Python interpreter
//--------------------------------------------------------------
class CFactory : public IFactory
{
public:
   CFactory();
   virtual ~CFactory();

   // IFactory implementation
   boost::shared_ptr<IPythonExecutable> createPythonExecutable() const override;
   boost::shared_ptr<shared::process::IProcess> createScriptProcess(boost::shared_ptr<yApi::IYInterpreterApi> api,
                                                                    int scriptInstanceId,
                                                                    const std::string& scriptPath,
                                                                    boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                    const boost::filesystem::path& interpreterPath,
                                                                    const std::string& scriptApiId,
                                                                    boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct) const override;
   // [END] IFactory implementation

protected:
   boost::shared_ptr<IScriptFile> createScriptFile(const std::string& scriptPath) const;
   boost::shared_ptr<shared::process::IProcessObserver> createScriptProcessObserver(int scriptInstanceId,
                                                                                    boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct) const;
   std::string createScriptLogger(int scriptInstanceId) const;
};
