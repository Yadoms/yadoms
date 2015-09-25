#include "stdafx.h"
#include "PythonExecutable.h"
#include "PythonException.hpp"
#include <shared/FileSystemExtension.h>
#include <shared/Log.h>


CPythonExecutable::CPythonExecutable()
   :m_executableDirectory(boost::filesystem::path()),
   m_found(findPythonDirectory(m_executableDirectory)),
   m_version(readPythonVersion(m_executableDirectory))
{
}

CPythonExecutable::~CPythonExecutable()
{
}

bool CPythonExecutable::found() const
{
   return m_found;
}

std::string CPythonExecutable::version() const
{
   return m_version;
}

bool CPythonExecutable::findPythonDirectory(boost::filesystem::path& pythonDirectory)
{
   boost::filesystem::path pythonInitialDir; // Empty directory to search in system path
   if (!readPythonVersion(pythonInitialDir).empty())
   {
      pythonDirectory = pythonInitialDir;
      return true;
   }
   YADOMS_LOG(debug) << "Python executable not found in the path";

#ifdef _WINDOWS
   // For Windows, search in %ProgramFiles%/python27 and %ProgramFiles(x86)%/python27 too

   pythonInitialDir = boost::filesystem::path(Poco::Environment::get("ProgramFiles"));
   pythonInitialDir = pythonInitialDir / "python27";

   if (!readPythonVersion(pythonInitialDir).empty())
   {
      pythonDirectory = pythonInitialDir;
      return true;
   }
   YADOMS_LOG(debug) << "Python executable not found in %ProgramFiles%/python27";


   pythonInitialDir = boost::filesystem::path(Poco::Environment::get("ProgramFiles(x86)"));
   pythonInitialDir = pythonInitialDir / "python27";

   if (!readPythonVersion(pythonInitialDir).empty())
   {
      pythonDirectory = pythonInitialDir;
      return true;
   }
   YADOMS_LOG(debug) << "Python executable not found in %ProgramFiles(x86)%/python27";

#endif //_WINDOWS

   YADOMS_LOG(error) << "Python executable not found";
   return false;
}

std::string CPythonExecutable::readPythonVersion(const boost::filesystem::path& initialDirectory)
{
   try
   {
      const boost::filesystem::path& command = initialDirectory / "python";

      Poco::Process::Args args;
      args.push_back("--version");

      Poco::Pipe outPipe;
      Poco::ProcessHandle processHandle = Poco::Process::launch(command.string(), args, NULL, &outPipe, &outPipe);
      processHandle.wait();

      Poco::PipeInputStream iStr(outPipe);
      std::ostringstream oStr;
      Poco::StreamCopier::copyStream(iStr, oStr);
      return oStr.str();
   }
   catch (Poco::Exception& ex)
   {
      YADOMS_LOG(debug) << "Unable to read Python version, " << ex.what();
      return std::string();
   }
}

void CPythonExecutable::startModule(boost::shared_ptr<const IScriptFile> scriptFile, const std::string& contextAccessorId, boost::shared_ptr<shared::script::ILogger> scriptLogger)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);

   try
   {
      const boost::filesystem::path& command = m_executableDirectory / "python";

      Poco::Process::Args args;
      args.push_back(std::string("-u")); // Make script outs unbuffered
      args.push_back(std::string("scriptCaller.py"));
      args.push_back(scriptFile->abslouteParentPath().string());
      args.push_back(scriptFile->module());
      args.push_back(contextAccessorId);

      Poco::Pipe outPipe, errPipe;
      m_process = boost::make_shared<Poco::ProcessHandle>(
         Poco::Process::launch(command.string(), args, shared::CFileSystemExtension::getModulePath().string(), NULL, &outPipe, &errPipe));

      boost::shared_ptr<Poco::PipeInputStream> moduleStdOut = boost::make_shared<Poco::PipeInputStream>(outPipe);
      boost::shared_ptr<Poco::PipeInputStream> moduleStdErr = boost::make_shared<Poco::PipeInputStream>(errPipe);
      m_StdOutRedirectingThread = boost::thread(&CPythonExecutable::stdRedirectWorker, this, scriptFile->module(), moduleStdOut, scriptLogger);
      m_StdErrRedirectingThread = boost::thread(&CPythonExecutable::stdRedirectWorker, this, scriptFile->module(), moduleStdErr, scriptLogger);
   }
   catch (Poco::Exception& ex)
   {
      throw CPythonException(std::string("Unable to start Python script, ") + ex.what());
   }
}

void CPythonExecutable::waitForStop()
{
   int returnCode = m_process->wait();
   if (returnCode != 0 && isError(returnCode))
      throw CPythonException("Script returned with error " + boost::lexical_cast<std::string>(returnCode));

   m_StdOutRedirectingThread.join();
   m_StdErrRedirectingThread.join();
}

void CPythonExecutable::interrupt()
{
   try
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_processMutex);
      if (!!m_process)
         Poco::Process::requestTermination(m_process->id());
   }
   catch (Poco::NotFoundException&)
   {
      // Nothing to do. This exception occurs when process is already stopped
   }
}

bool CPythonExecutable::isError(int code) const
{
#if defined WIN32
   if (static_cast<DWORD>(code) == STATUS_CONTROL_C_EXIT)
      return false;
#endif
   return true;
}

void CPythonExecutable::stdRedirectWorker(const std::string& ruleName,
   boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::script::ILogger> scriptLogger)
{
   char line[1024];
   YADOMS_LOG_CONFIGURE(ruleName + " rule");
   while (moduleStdOut->getline(line, sizeof(line)))
      scriptLogger->log(line);
}