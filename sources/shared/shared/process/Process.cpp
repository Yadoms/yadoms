#include "stdafx.h"
#include "Process.h"
#include "ProcessException.hpp"
#include <shared/FileSystemExtension.h>

//TODO utiliser cette classe pour les scripts
namespace shared
{
   namespace process
   {
      CProcess::CProcess(boost::shared_ptr<ICommandLine> commandLine,
                         const std::string& workingDirectory,
                         boost::shared_ptr<IEndOfProcessObserver> endOfProcessObserver,
                         boost::shared_ptr<ILogger> logger)
         : m_commandLine(commandLine),
           m_workingDirectory(workingDirectory),
           m_endOfProcessObserver(endOfProcessObserver),
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

            auto executableFullPath = CFileSystemExtension::getModulePath() / m_commandLine->workingDirectory() / m_commandLine->executable();

            if (!m_logger)
            {
               m_process = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(executableFullPath.string(),
                                                                                         args,
                                                                                         m_workingDirectory));
            }
            else
            {
               Poco::Pipe outPipe, errPipe;
               m_process = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(executableFullPath.string(),
                                                                                         args,
                                                                                         m_workingDirectory,
                                                                                         nullptr,
                                                                                         &outPipe,
                                                                                         &errPipe));

               auto moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
               auto moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
               m_StdOutRedirectingThread = boost::make_shared<boost::thread>(&CProcess::stdRedirectWorker,
                                                                             moduleStdOut,
                                                                             m_logger,
                                                                             boost::shared_ptr<std::string>());
               m_StdErrRedirectingThread = boost::make_shared<boost::thread>(&CProcess::stdRedirectWorker,
                                                                             moduleStdErr,
                                                                             m_logger,
                                                                             m_lastError);               
            }

            if (!!m_endOfProcessObserver)
               createEndOfProcessObserver();
		   }
		   catch (Poco::Exception& ex)
		   {
			   throw CProcessException(std::string("Unable to start plugin, ") + ex.what());
		   }
	   }

      void CProcess::createEndOfProcessObserver()
      {
         m_endOfProcessMonitorThread = boost::make_shared<boost::thread>(&CProcess::monitorThreaded, this);
      }

      void CProcess::monitorThreaded()
      {
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

         if (!!m_endOfProcessObserver)
            m_endOfProcessObserver->onEndOfProcess(m_returnCode, getError());
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

	   void CProcess::stdRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut,
	                                    boost::shared_ptr<ILogger> scriptLogger,
	                                    boost::shared_ptr<std::string> lastError)
	   {
		   char line[1024];
		   while (moduleStdOut->getline(line, sizeof(line)))
		   {
			   if (!!lastError)
				   *lastError += line;

			   // Remove EOL characters to log in script logger
			   auto len = strlen(line);
			   if (len > 1 && line[len - 2] == '\r' && line[len - 1] == '\n')
				   line[len - 2] = 0;
			   else if (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n'))
				   line[len - 1] = 0;

			   scriptLogger->log(line);
		   }
	   }
   }
} // namespace shared::process

