#include "stdafx.h"
#include "ScriptProcess.h"
#include <shared/Log.h>
#include "PythonException.hpp"
#include <windows/shared/FileSystemExtension.h>


CScriptProcess::CScriptProcess(
   boost::shared_ptr<IPythonExecutable> executable,
   boost::shared_ptr<const IScriptFile> scriptFile,
   const std::string& contextAccessorId,
   boost::shared_ptr<shared::script::ILogger> scriptLogger)
   :m_executable(executable),
   m_scriptFile(scriptFile),
   m_contextAccessorId(contextAccessorId),
   m_scriptLogger(scriptLogger)
{
   start();
}

CScriptProcess::~CScriptProcess()
{
   interrupt();
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
         Poco::Process::launch(m_executable->path().string(), args, shared::CFileSystemExtension::getModulePath().string(), NULL, &outPipe, &errPipe));

      boost::shared_ptr<Poco::PipeInputStream> moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
      boost::shared_ptr<Poco::PipeInputStream> moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
      m_StdOutRedirectingThread = boost::thread(&CScriptProcess::stdRedirectWorker, this, m_scriptFile->module(), moduleStdOut, m_scriptLogger);
      m_StdErrRedirectingThread = boost::thread(&CScriptProcess::stdRedirectWorker, this, m_scriptFile->module(), moduleStdErr, m_scriptLogger);
   }
   catch (Poco::Exception& ex)
   {
      throw CPythonException(std::string("Unable to start Python script, ") + ex.what());
   }
}

void CScriptProcess::interrupt()
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
   if (!m_process)
      return 0;

   try
   {
      return Poco::Process::wait(*m_process);
   }
   catch (Poco::SystemException&)
   {
      // Process was probably killed (==> stopped by user)
      return 0;
   }
}

void CScriptProcess::stdRedirectWorker(const std::string& ruleName,
   boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::script::ILogger> scriptLogger)
{
   char line[1024];
   YADOMS_LOG_CONFIGURE(ruleName + " rule");
   while (moduleStdOut->getline(line, sizeof(line)))
      scriptLogger->log(line);
}