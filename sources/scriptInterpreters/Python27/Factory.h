#pragma once
#include "IFactory.h"
#include "IScriptFile.h"

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
   boost::shared_ptr<shared::process::IProcessObserver> createScriptProcessObserver(int scriptInstanceId,
                                                                                    boost::function2<void, bool, int> onInstanceStateChangedFct) const override;
   boost::shared_ptr<shared::process::IExternalProcessLogger> createScriptLogger(int scriptInstanceId) const override;
   boost::shared_ptr<shared::process::IProcess> createScriptProcess(const std::string& scriptPath,
                                                                    boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                    const boost::filesystem::path& interpreterPath,
                                                                    boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                                                                    const std::string& scriptApiId,
                                                                    boost::shared_ptr<shared::process::IProcessObserver> processObserver) const override;
   // [END] IFactory implementation

protected:
   boost::shared_ptr<IScriptFile> createScriptFile(const std::string& scriptPath) const;
};

