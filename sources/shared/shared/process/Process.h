#pragma once
#include <shared/Export.h>
#include "ILogger.h"
#include "IProcess.h"
#include "ICommandLine.h"
#include "IEndOfProcessObserver.h"
#include <Poco/Process.h>
#include <Poco/PipeStream.h>

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Python executable
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CProcess : public IProcess
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] commandLine             Process command line
         /// \param[in] workingDirectory        Working directory to use. If null, use the current working directory.
         /// \param[in] endOfProcessObserver    The end of process event observer (can be NULL is no observer needed)
         /// \param[in] logger                  Process out/err logger (can be NULL is no logger needed)
         /// \throw CPluginException if error //TODO revoir le type
         //--------------------------------------------------------------
         CProcess(boost::shared_ptr<ICommandLine> commandLine,
                  const std::string& workingDirectory,
                  boost::shared_ptr<IEndOfProcessObserver> endOfProcessObserver,
                  boost::shared_ptr<ILogger> logger);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CProcess();

         // shared::process::IProcess Implementation
         void kill() override;
         int getReturnCode() const override;
         std::string getError() const override;
         // [END] shared::process::IProcess Implementation

      protected:
         //--------------------------------------------------------------
         /// \brief	Start a module (in separated process)
         /// \throw CPythonException if error
         //--------------------------------------------------------------
         void start();

         //-----------------------------------------------------
         ///\brief               Create the process observer 
         //-----------------------------------------------------
         void createEndOfProcessObserver();

         //-----------------------------------------------------
         ///\brief               Rule montoring thread function
         //-----------------------------------------------------
         void monitorThreaded();

         //--------------------------------------------------------------
         /// \brief	Thread redirecting standard outputs
         /// \param[in] moduleStdOut      StdOut to redirect
         /// \param[in] targetStream      Target stream
         /// \param[inout] lastError      Last error string
         //--------------------------------------------------------------
         static void stdRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut,
                                       boost::shared_ptr<ILogger> scriptLogger,
                                       boost::shared_ptr<std::string> lastError);

      private:
         //--------------------------------------------------------------
         /// \brief	The command line to execute
         //--------------------------------------------------------------
         boost::shared_ptr<ICommandLine> m_commandLine;

         //--------------------------------------------------------------
         /// \brief	The working directory
         //--------------------------------------------------------------
         const std::string m_workingDirectory;

         //--------------------------------------------------------------
         /// \brief	The process of the running script, and its mutex
         //--------------------------------------------------------------
         boost::shared_ptr<Poco::ProcessHandle> m_process;
         mutable boost::recursive_mutex m_processMutex;

         //--------------------------------------------------------------
         ///\brief   The end of process observer
         //--------------------------------------------------------------
         boost::shared_ptr<IEndOfProcessObserver> m_endOfProcessObserver;
         boost::shared_ptr<boost::thread> m_endOfProcessMonitorThread;

         //--------------------------------------------------------------
         ///\brief   The logger
         //--------------------------------------------------------------
         boost::shared_ptr<ILogger> m_logger;

         //--------------------------------------------------------------
         /// \brief	Thread redirecting standard outputs
         //--------------------------------------------------------------
         boost::shared_ptr<boost::thread> m_StdOutRedirectingThread;
         boost::shared_ptr<boost::thread> m_StdErrRedirectingThread;

         //--------------------------------------------------------------
         /// \brief	The process return code
         //--------------------------------------------------------------
         int m_returnCode;

         //--------------------------------------------------------------
         /// \brief	The last error
         //--------------------------------------------------------------
         boost::shared_ptr<std::string> m_lastError;
      };
   }
} // namespace shared::process

