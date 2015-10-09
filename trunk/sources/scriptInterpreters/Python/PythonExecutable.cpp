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
