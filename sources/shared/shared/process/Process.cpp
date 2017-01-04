#include "stdafx.h"
#include "Process.h"
#include <shared/Log.h>

namespace shared
{
   namespace process
   {
      CProcess::CProcess(boost::shared_ptr<ICommandLine> commandLine,
                         boost::shared_ptr<IProcessObserver> processObserver,
                         boost::shared_ptr<IExternalProcessLogger> logger)
         : m_commandLine(commandLine),
           m_processObserver(processObserver),
           m_logger(logger),
           m_returnCode(0),
           m_lastError(boost::make_shared<std::string>())
      {
         start();
      }

      CProcess::~CProcess()
      {
         CProcess::kill();
      }

      void CProcess::start()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);

         try
         {
            Poco::Process::Args args;
            for (auto cmdLineArg = m_commandLine->args().begin(); cmdLineArg != m_commandLine->args().end(); ++cmdLineArg)
               args.push_back(*cmdLineArg);


            YADOMS_LOG(debug) << "Start process " << m_commandLine->executable() << " from " << m_commandLine->workingDirectory();

            if (!m_logger)
            {
               m_process = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(m_commandLine->executable().string(),
                                                                                         args,
                                                                                         m_commandLine->workingDirectory().string()));
            }
            else
            {
               Poco::Pipe outPipe, errPipe;
               m_process = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(m_commandLine->executable().string(),
                                                                                         args,
                                                                                         m_commandLine->workingDirectory().string(),
                                                                                         nullptr,
                                                                                         &outPipe,
                                                                                         &errPipe));

               auto moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
               auto moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
               m_StdOutRedirectingThread = boost::make_shared<boost::thread>(&CProcess::stdOutRedirectWorker,
                                                                             moduleStdOut,
                                                                             m_logger);
               m_StdErrRedirectingThread = boost::make_shared<boost::thread>(&CProcess::stdErrRedirectWorker,
                                                                             moduleStdErr,
                                                                             m_logger,
                                                                             m_lastError);
            }

            if (!!m_processObserver)
               createProcessObserver();
         }
         catch (Poco::Exception& ex)
         {
            auto error = std::string("Unable to start process, ") + ex.what();
            YADOMS_LOG(error) << error;
            m_processObserver->onFinish(ex.code(),
                                        error);
         }
      }

      void CProcess::createProcessObserver()
      {
         m_processMonitorThread = boost::make_shared<boost::thread>(&CProcess::monitorThreaded, this);
      }

      void CProcess::monitorThreaded()
      {
         if (!!m_processObserver)
            m_processObserver->onStart();

         if (!m_process)
            m_returnCode = 0;
         else
         {
            try
            {
               m_returnCode = Poco::Process::wait(*m_process);
            }
            catch (Poco::SystemException&)
            {
               // Process was probably killed (==> stopped by user)
               m_returnCode = 0;
            }
         }

         if (!!m_StdOutRedirectingThread)
            m_StdOutRedirectingThread->join();
         if (!!m_StdErrRedirectingThread)
            m_StdErrRedirectingThread->join();

         if (!!m_processObserver)
            m_processObserver->onFinish(m_returnCode, getError());
      }

      void CProcess::kill()
      {
         try
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);
            if (!!m_process)
            {
               Poco::Process::kill(*m_process);

               if (Poco::Process::isRunning(*m_process))
                  m_process->wait();
            }
         }
         catch (Poco::NotFoundException&)
         {
            // Nothing to do. This exception can occur when process is already stopped
         }
         catch (Poco::RuntimeException&)
         {
            // Nothing to do. This exception can occur when process is already stopped
         }
      }

      int CProcess::getReturnCode() const
      {
         return m_returnCode;
      }

      std::string CProcess::getError() const
      {
         return *m_lastError;
      }

      void CProcess::stdOutRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut,
                                          boost::shared_ptr<IExternalProcessLogger> scriptLogger)
      {
         scriptLogger->init();
         scriptLogger->information("#### START ####");

         char line[1024];
         while (moduleStdOut->getline(line, sizeof(line)))
         {
            scriptLogger->information(line);
         }
      }

      void CProcess::stdErrRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdErr,
                                          boost::shared_ptr<IExternalProcessLogger> scriptLogger,
                                          boost::shared_ptr<std::string> lastError)
      {
         scriptLogger->init();

         char line[1024];
         while (moduleStdErr->getline(line, sizeof(line)))
         {
            if (!!lastError)
               *lastError += line;

            scriptLogger->error(line);
         }
      }
   }
} // namespace shared::process


