#include "stdafx.h"
#include "Process.h"
#include "PluginException.hpp"
#include <shared/FileSystemExtension.h>

namespace pluginSystem
{
   CProcess::CProcess(boost::shared_ptr<ICommandLine> commandLine,
                      boost::shared_ptr<shared::process::ILogger> logger)
      : m_commandLine(commandLine), m_logger(logger)
   {
      start();
   }

   CProcess::~CProcess()
   {
      CProcess::interrupt();
   }

   void CProcess::start()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);

      try
      {
         Poco::Process::Args args;
         for (auto cmdLineArg = m_commandLine->args().begin(); cmdLineArg != m_commandLine->args().end(); ++cmdLineArg)
            args.push_back(*cmdLineArg);


         Poco::Pipe outPipe, errPipe;
         auto executableFullPath = shared::CFileSystemExtension::getModulePath() / m_commandLine->workingDirectory() / m_commandLine->executable();
         m_process = boost::make_shared<Poco::ProcessHandle>(Poco::Process::launch(executableFullPath.string(),
                                                                                   args,
                                                                                   shared::CFileSystemExtension::getModulePath().string(),
                                                                                   nullptr,
                                                                                   &outPipe,
                                                                                   &errPipe));

         auto moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
         auto moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
         m_StdOutRedirectingThread = boost::thread(&CProcess::stdRedirectWorker,
                                                   moduleStdOut,
                                                   m_logger,
                                                   boost::shared_ptr<std::string>());
         m_StdErrRedirectingThread = boost::thread(&CProcess::stdRedirectWorker,
                                                   moduleStdErr,
                                                   m_logger,
                                                   m_lastError);
      }
      catch (Poco::Exception& ex)
      {
         throw CPluginException(std::string("Unable to start plugin, ") + ex.what());
      }
   }

   void CProcess::interrupt()
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

      m_StdOutRedirectingThread.join();
      m_StdErrRedirectingThread.join();
   }

   int CProcess::waitForStop()
   {
      int returnCode;

      if (!m_process)
         returnCode = 0;
      else
      {
         try
         {
            returnCode = Poco::Process::wait(*m_process);
         }
         catch (Poco::SystemException&)
         {
            // Process was probably killed (==> stopped by user)
            returnCode = 0;
         }
      }

      m_StdOutRedirectingThread.join();
      m_StdErrRedirectingThread.join();

      return returnCode;
   }

   std::string CProcess::getError() const
   {
      return *m_lastError;
   }

   void CProcess::stdRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut,
                                    boost::shared_ptr<shared::process::ILogger> scriptLogger,
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
} // namespace pluginSystem

