#include "stdafx.h"
#include "ScriptProcess.h"
#include <shared/Log.h>
#include "PythonException.hpp"
#include "FileSystemPathHelper.h"


CScriptProcess::CScriptProcess(
   boost::shared_ptr<IPythonExecutable> executable,
   boost::shared_ptr<const IScriptFile> scriptFile,
   const std::string& contextAccessorId,
   boost::shared_ptr<shared::process::ILogger> scriptLogger)
   :m_executable(executable),
   m_scriptFile(scriptFile),
   m_contextAccessorId(contextAccessorId),
   m_lastError(boost::make_shared<std::string>()),
   m_scriptLogger(scriptLogger)
{
   start();
}

CScriptProcess::~CScriptProcess()
{
   CScriptProcess::kill();
}

void CScriptProcess::start()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);

   try
   {
      Poco::Process::Args args;
      args.push_back(std::string("-u")); // Make script outs unbuffered
      args.push_back(std::string("scriptCaller.py"));
      args.push_back(m_scriptFile->abslouteParentPath().string());
      args.push_back(m_scriptFile->module());
      args.push_back(m_contextAccessorId);

      Poco::Pipe outPipe, errPipe;
      m_process = boost::make_shared<Poco::ProcessHandle>(
         Poco::Process::launch(m_executable->path().string(), args, CFileSystemPathHelper::getExecutingPath().string(), NULL, &outPipe, &errPipe));

      boost::shared_ptr<Poco::PipeInputStream> moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
      boost::shared_ptr<Poco::PipeInputStream> moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
      m_StdOutRedirectingThread = boost::thread(&CScriptProcess::stdRedirectWorker, this, m_scriptFile->module(), moduleStdOut, m_scriptLogger, boost::shared_ptr<std::string>());
      m_StdErrRedirectingThread = boost::thread(&CScriptProcess::stdRedirectWorker, this, m_scriptFile->module(), moduleStdErr, m_scriptLogger, m_lastError);
   }
   catch (Poco::Exception& ex)
   {
      throw CPythonException(std::string("Unable to start Python script, ") + ex.what());
   }
}

void CScriptProcess::kill()
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

int CScriptProcess::waitForStop()
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

std::string CScriptProcess::getError() const
{
   return *m_lastError;
}

void CScriptProcess::stdRedirectWorker(const std::string& ruleName,
   boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::process::ILogger> scriptLogger, boost::shared_ptr<std::string> lastError)
{
   char line[1024];
   YADOMS_LOG_CONFIGURE(ruleName + " rule");
   while (moduleStdOut->getline(line, sizeof(line)))
   {
      if (!!lastError)
         *lastError += line;

      // Remove EOL characters to log in script logger
      size_t len = strlen(line);
      if (len > 1 && line[len - 2] == '\r' && line[len - 1] == '\n')
         line[len - 2] = 0;
      else if (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n'))
         line[len - 1] = 0;

      scriptLogger->log(line);
   }
}