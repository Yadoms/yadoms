#pragma once
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
      class CProcess : public shared::process::IProcess
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] commandLine             Process command line
         /// \param[in] endOfProcessObserver    The end of process event observer (can be NULL is no observer needed)
         /// \param[in] logger                  Process out/err logger (can be NULL is no logger needed)
         /// \throw CPluginException if error //TODO revoir le type
         //--------------------------------------------------------------
         CProcess(boost::shared_ptr<ICommandLine> commandLine,
                  boost::shared_ptr<shared::process::IEndOfProcessObserver> endOfProcessObserver,
                  boost::shared_ptr<shared::process::ILogger> logger);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CProcess();

         // shared::process::IProcess Implementation
         void kill() override;
         int waitForStop() override;
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
         ///\param[in] process               The process to monitor
         ///\param[in] endOfProcessObserver  The end of process event observer
         //-----------------------------------------------------
         void monitorThreaded(boost::shared_ptr<shared::process::IProcess> process,
                              boost::shared_ptr<shared::process::IEndOfProcessObserver> endOfProcessObserver) const;

         //--------------------------------------------------------------
         /// \brief	Thread redirecting standard outputs
         /// \param[in] moduleStdOut      StdOut to redirect
         /// \param[in] targetStream      Target stream
         /// \param[inout] lastError      Last error string
         //--------------------------------------------------------------
         static void stdRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut,
                                       boost::shared_ptr<shared::process::ILogger> scriptLogger,
                                       boost::shared_ptr<std::string> lastError);

      private:
         boost::shared_ptr<ICommandLine> m_commandLine;

         //--------------------------------------------------------------
         /// \brief	The process of the running script, and its mutex
         //--------------------------------------------------------------
         boost::shared_ptr<Poco::ProcessHandle> m_process;
         mutable boost::recursive_mutex m_processMutex;

         //--------------------------------------------------------------
         ///\brief   The end of process observer
         //--------------------------------------------------------------
         boost::shared_ptr<shared::process::IEndOfProcessObserver> m_endOfProcessObserver;
         boost::shared_ptr<boost::thread> m_endOfProcessMonitorThread;

         //--------------------------------------------------------------
         ///\brief   The logger
         //--------------------------------------------------------------
         boost::shared_ptr<shared::process::ILogger> m_logger;

         //--------------------------------------------------------------
         /// \brief	Thread redirecting standard outputs
         //--------------------------------------------------------------
         boost::thread m_StdOutRedirectingThread;
         boost::thread m_StdErrRedirectingThread;

         //--------------------------------------------------------------
         /// \brief	The last error
         //--------------------------------------------------------------
         boost::shared_ptr<std::string> m_lastError;
      };
   }
} // namespace shared::process

