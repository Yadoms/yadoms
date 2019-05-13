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
           m_returnCode(0),
           m_lastError(boost::make_shared<std::string>())
      {
         start(logger);
      }

      CProcess::~CProcess()
      {
         CProcess::kill();
      }

      void CProcess::start(boost::shared_ptr<IExternalProcessLogger> logger)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);

         try
         {
            Poco::Process::Args args;

			auto & ar = m_commandLine->args();
			std::copy(ar.begin(), ar.end(),	std::back_inserter(args));

            YADOMS_LOG(debug) << "Start process " << m_commandLine->executable() << " from " << m_commandLine->workingDirectory();

            Poco::Pipe outPipe, errPipe;
            m_processHandle = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(m_commandLine->executable().string(),
                                                                                            args,
                                                                                            m_commandLine->workingDirectory().string(),
                                                                                            nullptr,
                                                                                            logger ? &outPipe : nullptr,
                                                                                            logger ? &errPipe : nullptr));

            if (logger)
            {
               auto moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
               auto moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
               m_StdOutRedirectingThread = boost::make_shared<boost::thread>(&CProcess::stdOutRedirectWorker,
                                                                             moduleStdOut,
                                                                             logger);
               m_StdErrRedirectingThread = boost::make_shared<boost::thread>(&CProcess::stdErrRedirectWorker,
                                                                             moduleStdErr,
                                                                             logger,
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
         const auto processHandle = *m_processHandle;
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
                                          boost::shared_ptr<IExternalProcessLogger> logger)
      {
         logger->init();
         logger->information("#### START ####");

         char line[1024];
         while (moduleStdOut->getline(line, sizeof line))
         {
            logger->information(line);
         }
      }

      void CProcess::stdErrRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdErr,
                                          boost::shared_ptr<IExternalProcessLogger> logger,
                                          boost::shared_ptr<std::string> lastError)
      {
         logger->init();

         char line[1024];
         while (moduleStdErr->getline(line, sizeof line))
         {
            if (!!lastError)
               *lastError += line;

            logger->error(line);
         }
      }
   }
} // namespace shared::process
