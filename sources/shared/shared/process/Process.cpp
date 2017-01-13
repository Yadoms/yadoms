#include "stdafx.h"
#include "Process.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>

namespace shared
{
   namespace process
   {
      CProcess::CProcess(boost::shared_ptr<ICommandLine> commandLine,
                         boost::shared_ptr<IProcessObserver> processObserver,
                         const std::string& logger)
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

            Poco::Pipe outPipe, errPipe;
            m_processHandle = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(m_commandLine->executable().string(),
                                                                                            args,
                                                                                            m_commandLine->workingDirectory().string(),
                                                                                            nullptr,
                                                                                            m_logger.empty() ? nullptr : &outPipe,
                                                                                            m_logger.empty() ? nullptr : &errPipe));

            if (!m_logger.empty())
            {
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
            throw std::runtime_error(std::string("Unable to start process, ") + ex.what());
         }
      }

      void CProcess::createProcessObserver()
      {
         m_processMonitorThread = boost::make_shared<boost::thread>(&CProcess::monitorThreaded,
                                                                    this);
      }

      void CProcess::monitorThreaded()
      {
         // Work on local copy of the process handle to avoid to lock mutex while wait for process end
         m_processMutex.lock();
         auto processHandle = *m_processHandle;
         m_processMutex.unlock();

         if (!!m_processObserver)
            m_processObserver->onStart();

         if (!Poco::Process::isRunning(processHandle))
            m_returnCode = 0;
         else
         {
            try
            {
               m_returnCode = Poco::Process::wait(processHandle);
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
            m_processObserver->onFinish(m_returnCode,
                                        getError());
      }

      void CProcess::kill()
      {
         try
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);
            if (Poco::Process::isRunning(*m_processHandle))
            {
               Poco::Process::kill(*m_processHandle);

               if (Poco::Process::isRunning(*m_processHandle))
                  m_processHandle->wait();
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

         if (m_processMonitorThread)
            m_processMonitorThread->join();
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
                                          const std::string& scriptLogger)
      {
         auto& logger = Poco::Logger::get(scriptLogger);
         YADOMS_LOG_CONFIGURE(scriptLogger);
         logger.information("#### START ####");

         char line[1024];
         while (moduleStdOut->getline(line, sizeof(line)))
         {
            const auto l(CStringExtension::removeEol(line));
            if (!l.empty())
            {
               logger.information(l);

               //TODO : que vient faire ce cas particulier sur les plugins dans la classe commune CProcess ?
               if (scriptLogger != "plugin") //if script logger, log it in yadoms logger
                  YADOMS_LOG(information) << l;
            }
         }
      }

      void CProcess::stdErrRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdErr,
                                          const std::string& scriptLogger,
                                          boost::shared_ptr<std::string> lastError)
      {
         auto& logger = Poco::Logger::get(scriptLogger);
         YADOMS_LOG_CONFIGURE(scriptLogger);

         char line[1024];
         while (moduleStdErr->getline(line, sizeof(line)))
         {
            if (!!lastError)
               *lastError += line;

            const auto l(CStringExtension::removeEol(line));
            if (!l.empty())
            {
               logger.error(l);

               //TODO : que vient faire ce cas particulier sur les plugins dans la classe commune CProcess ?
               if (scriptLogger != "plugin")
                  YADOMS_LOG(error) << l;
            }
         }
      }
   }
} // namespace shared::process


