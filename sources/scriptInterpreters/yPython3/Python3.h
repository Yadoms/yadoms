#pragma once
#include "IPythonExecutable.h"
#include <interpreter_cpp_api/IInterpreter.h>
#include <shared/process/IProcess.h>
#include "IFactory.h"
#include "IEventScriptStopped.h"

//--------------------------------------------------------------
/// \brief	Python 3 interpreter
//--------------------------------------------------------------
class CPython3 : public interpreter_cpp_api::IInterpreter
{
public:
   CPython3();
   virtual ~CPython3();

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
   void startScript(int scriptInstanceId,
                    const boost::filesystem::path& scriptPath,
                    const std::string& scriptApiId,
                    const boost::filesystem::path& scriptLogPath);
   void stopScript(int scriptInstanceId);
   void onScriptStopped(const boost::shared_ptr<const IEventScriptStopped> eventStopped);
   void onStopRequested();
   void deleteScriptLog(int scriptInstanceId,
                        const boost::filesystem::path& scriptLogPath);

private:
   boost::shared_ptr<IFactory> m_factory;
   boost::shared_ptr<yApi::IYInterpreterApi> m_api;
   boost::shared_ptr<IPythonExecutable> m_pythonExecutable;

   mutable boost::recursive_mutex m_processesMutex;
   std::map<int, boost::shared_ptr<shared::process::IProcess>> m_scriptProcesses;
};
