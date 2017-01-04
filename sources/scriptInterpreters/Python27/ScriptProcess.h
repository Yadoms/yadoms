#pragma once
#include "IPythonExecutable.h"
#include <shared/process/IExternalProcessLogger.h>
#include <shared/process/IProcess.h>
#include "IScriptFile.h"
#include <shared/process/ICommandLine.h>
#include <shared/process/IProcessObserver.h>

//--------------------------------------------------------------
/// \brief	Python process
//--------------------------------------------------------------
class CScriptProcess : public shared::process::IProcess
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] executable  Python executable to call to start script
   /// \param[in] interpreterPath  The current library path
   /// \param[in] scriptFile The script file to execute
   /// \param[in] scriptApiId The script Api ID, used to interact with Yadoms
   /// \param[in] scriptLogger The script logger
   /// \param[in] processObserver The process observer
   //--------------------------------------------------------------
   CScriptProcess(boost::shared_ptr<IPythonExecutable> executable,
                  const boost::filesystem::path& interpreterPath,
                  boost::shared_ptr<const IScriptFile> scriptFile,
                  const std::string& scriptApiId,
                  boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                  boost::shared_ptr<shared::process::IProcessObserver> processObserver);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CScriptProcess();

   // shared::process::IProcess Implementation
   void kill() override;
   int getReturnCode() const override;
   std::string getError() const override;
   // [END] shared::process::IProcess Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Start a module
   //--------------------------------------------------------------
   void start();

   boost::shared_ptr<shared::process::ICommandLine> createCommandLine(const std::string& apiIdentifier) const;

private:
   //--------------------------------------------------------------
   /// \brief	The Python executable to call to start script
   //--------------------------------------------------------------
   boost::shared_ptr<IPythonExecutable> m_executable;

   //--------------------------------------------------------------
   /// \brief	The current library path
   //--------------------------------------------------------------
   const boost::filesystem::path& m_interpreterPath;

   //--------------------------------------------------------------
   /// \brief	The script file to execute
   //--------------------------------------------------------------
   boost::shared_ptr<const IScriptFile> m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	The script api ID
   //--------------------------------------------------------------
   const std::string& m_scriptApiId;

   //--------------------------------------------------------------
   /// \brief	The script logger
   //--------------------------------------------------------------
   boost::shared_ptr<shared::process::IExternalProcessLogger> m_scriptLogger;

   //--------------------------------------------------------------
   /// \brief	Object to notify when process stops
   //--------------------------------------------------------------
   boost::shared_ptr<shared::process::IProcessObserver> m_processObserver;

   //--------------------------------------------------------------
   /// \brief	The process
   //--------------------------------------------------------------
   boost::shared_ptr<IProcess> m_process;
};

