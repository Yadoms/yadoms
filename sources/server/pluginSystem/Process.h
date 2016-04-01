#pragma once
#include <shared/process/ILogger.h>
#include <shared/process/IProcess.h>
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include "ICommandLine.h"

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Python executable
   //--------------------------------------------------------------
   class CProcess : public shared::process::IProcess
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] commandLine    Process command line
      /// \param[in] logger         Process out/err logger
      /// \throw CPluginException if error
      //--------------------------------------------------------------
      CProcess(boost::shared_ptr<ICommandLine> commandLine, boost::shared_ptr<shared::process::ILogger> logger);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CProcess();

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
      boost::shared_ptr<ICommandLine> m_commandLine;
      boost::shared_ptr<shared::process::ILogger> m_logger;

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

} // namespace pluginSystem

