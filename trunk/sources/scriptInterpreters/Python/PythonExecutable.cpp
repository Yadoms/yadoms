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

boost::filesystem::path CPythonExecutable::path() const
{
   return m_executableDirectory / "python";
}

bool CPythonExecutable::findPythonDirectory(boost::filesystem::path& pythonDirectory)
{
   if (isPythonIn(boost::filesystem::path(), pythonDirectory))// Empty directory to search in system path
      return true;

#ifdef _WINDOWS
   // For Windows, search in c:/python27 first (because it's the default installation path),
   // then in %ProgramFiles%/python27 and in %ProgramFiles(x86)%/python27

   if (
      isPythonIn(boost::filesystem::path("C:") / "python27", pythonDirectory) ||
      isPythonIn(boost::filesystem::path(Poco::Environment::get("ProgramFiles")) / "python27", pythonDirectory) ||
      isPythonIn(boost::filesystem::path(Poco::Environment::get("ProgramFiles(x86)")) / "python27", pythonDirectory))
      return true;

#endif //_WINDOWS

   YADOMS_LOG(error) << "Python executable not found";
   return false;
}

bool CPythonExecutable::isPythonIn(const boost::filesystem::path& directory, boost::filesystem::path& pythonDirectory)
{
   if (!readPythonVersion(directory).empty())
   {
      YADOMS_LOG(debug) << "Python executable found in " << (directory.empty() ? "the path" : directory.string());
      pythonDirectory = directory;
      return true;
   }
   YADOMS_LOG(debug) << "Python executable not found in " << (directory.empty() ? "the path" : directory.string());
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
