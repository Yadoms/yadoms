#pragma once
#include "IPythonExecutable.h"
#include <interpreter_cpp_api/IInterpreter.h>
#include <shared/process/IExternalProcessLogger.h>
#include <shared/process/IProcess.h>
#include <shared/process/IProcessObserver.h>


// Shortcut to yPluginApi namespace
namespace yApi = shared::script::yInterpreterApi;

//--------------------------------------------------------------
/// \brief	Python interpreter
//--------------------------------------------------------------
class CPython27 : public interpreter_cpp_api::IInterpreter
{
public:
   CPython27();
   virtual ~CPython27();

   // IInterpreter implementation
   void doWork(boost::shared_ptr<yApi::IYInterpreterApi> api) override;
   // [END] IInterpreter implementation

protected:
   const boost::filesystem::path& getInterpreterPath() const;
   const std::string& getScriptTemplate() const;
   bool isAvailable() const;
   std::string loadScriptContent(const std::string& scriptPath) const;
   static void saveScriptContent(const std::string& scriptPath,
                                 const std::string& content);
   boost::shared_ptr<shared::process::IProcess> createProcess(const std::string& scriptPath,
                                                              boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                                                              const std::string& scriptApiId,
                                                              boost::shared_ptr<shared::process::IProcessObserver> processObserver) const;

private:
   boost::shared_ptr<yApi::IYInterpreterApi> m_api;
   boost::shared_ptr<IPythonExecutable> m_pythonExecutable;
};

