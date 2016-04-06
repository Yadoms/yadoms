#pragma once
#include "IPythonExecutable.h"
#include <shared/process/ILogger.h>
#include <shared/process/IProcess.h>
#include "IScriptFile.h"

//--------------------------------------------------------------
/// \brief	Python executable
//--------------------------------------------------------------
class CScriptProcess : public shared::process::IProcess
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] executable  Python executable to call to start script
   /// \param[in] scriptFile The script file to execute
   /// \param[in] contextAccessorId The context accessor ID, used by script to interact with Yadoms
   /// \param[in] scriptLogger The script logger
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   CScriptProcess(boost::shared_ptr<IPythonExecutable> executable,
      boost::shared_ptr<const IScriptFile> scriptFile,
      const std::string& contextAccessorId,
      boost::shared_ptr<shared::process::ILogger> scriptLogger);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CScriptProcess();

   // shared::process::IProcess Implementation
   virtual void interrupt();
   virtual int waitForStop();
   virtual std::string getError() const;
   // [END] shared::process::IProcess Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Start a module (in separated process)
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   void start();

   //--------------------------------------------------------------
   /// \brief	Thread redirecting standard outputs
   /// \param[in] ruleName          Rule name used for log identification
   /// \param[in] moduleStdOut      StdOut to redirect
   /// \param[in] targetStream      Target stream
   /// \param[inout] lastError      Last error string
   //--------------------------------------------------------------
   void stdRedirectWorker(const std::string& ruleName,
      boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::process::ILogger> scriptLogger, boost::shared_ptr<std::string> lastError);

private:
   //--------------------------------------------------------------
   /// \brief	The Python executable to call to start script
   //--------------------------------------------------------------
   boost::shared_ptr<IPythonExecutable> m_executable;

   //--------------------------------------------------------------
   /// \brief	The script file to execute
   //--------------------------------------------------------------
   boost::shared_ptr<const IScriptFile> m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	The context accessor ID, used by script to interact with Yadoms
   //--------------------------------------------------------------
   const std::string& m_contextAccessorId;

   //--------------------------------------------------------------
   /// \brief	The last error
   //--------------------------------------------------------------
   boost::shared_ptr<std::string> m_lastError;

   //--------------------------------------------------------------
   /// \brief	The script logger
   //--------------------------------------------------------------
   boost::shared_ptr<shared::process::ILogger> m_scriptLogger;

   //--------------------------------------------------------------
   /// \brief	The process of the running script, and its mutex
   //--------------------------------------------------------------
   boost::shared_ptr<Poco::ProcessHandle> m_process;
   mutable boost::recursive_mutex m_processMutex;

   //--------------------------------------------------------------
   /// \brief	Thread redirecting standard outputs
   //--------------------------------------------------------------
   boost::thread m_StdOutRedirectingThread;
   boost::thread m_StdErrRedirectingThread;
};



