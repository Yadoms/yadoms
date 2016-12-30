#pragma once
#include <shared/Export.h>
#include "ILogger.h"
#include "IProcess.h"
#include "ICommandLine.h"
#include "IProcessObserver.h"
#include <Poco/Process.h>
#include <Poco/PipeStream.h>

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Python executable
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT [[deprecated("Use CProcess instead")]] CProcessDeprecated : public IProcess
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] commandLine             Process command line
         /// \param[in] processObserver         The process life observer (can be NULL is no observer needed)
         /// \param[in] logger                  Process out/err logger (can be NULL is no logger needed)
         /// \throw CProcessException if error
         //--------------------------------------------------------------
         CProcessDeprecated(boost::shared_ptr<ICommandLine> commandLine,
                  boost::shared_ptr<IProcessObserver> processObserver,
                  boost::shared_ptr<ILogger> logger);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CProcessDeprecated();

         // shared::process::IProcess Implementation
         void kill() override;
         int getReturnCode() const override;
         std::string getError() const override;
         // [END] shared::process::IProcess Implementation

      protected:
         //--------------------------------------------------------------
         /// \brief	Start a module (in separated process)
         //--------------------------------------------------------------
         void start();

         //-----------------------------------------------------
         ///\brief               Create the process observer 
         //-----------------------------------------------------
         void createProcessObserver();

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
         static void stdOutRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut,
                                          boost::shared_ptr<ILogger> scriptLogger);
         static void stdErrRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdErr,
                                          boost::shared_ptr<ILogger> scriptLogger,
                                          boost::shared_ptr<std::string> lastError);

      private:
         //--------------------------------------------------------------
         /// \brief	The command line to execute
         //--------------------------------------------------------------
         boost::shared_ptr<ICommandLine> m_commandLine;

         //--------------------------------------------------------------
         /// \brief	The process of the running script, and its mutex
         //--------------------------------------------------------------
         boost::shared_ptr<Poco::ProcessHandle> m_process;
         mutable boost::recursive_mutex m_processMutex;

         //--------------------------------------------------------------
         ///\brief   The end of process observer
         //--------------------------------------------------------------
         boost::shared_ptr<IProcessObserver> m_processObserver;
         boost::shared_ptr<boost::thread> m_processMonitorThread;

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

