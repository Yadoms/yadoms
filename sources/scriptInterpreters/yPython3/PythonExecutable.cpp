#include "stdafx.h"
#include "PythonExecutable.h"
#include "PythonExecutablePath.h"
#include <shared/Executable.h>
#include <shared/Log.h>

CPythonExecutable::CPythonExecutable()
   : m_inSystemPath(false),
     m_found(findPythonDirectory(m_executableDirectory, m_inSystemPath)),
     m_version(readPythonVersion(m_executableDirectory))
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

bool CPythonExecutable::inSystemPath() const
{
   return m_inSystemPath;
}

boost::filesystem::path CPythonExecutable::path() const
{
   return m_executableDirectory / filename();
}

std::string CPythonExecutable::filename() const
{
   return shared::CExecutable::ToFileName(CPythonExecutablePath::getExecutableName());
}

bool CPythonExecutable::findPythonDirectory(boost::filesystem::path& pythonDirectory,
                                            bool& inSystemPath)
{
   //retrieve common path for the system
   std::vector<boost::filesystem::path> commonPaths;
   CPythonExecutablePath::getCommonPythonPaths(commonPaths);

   for (const auto& path : commonPaths)
   {
      if (isPythonIn(path, pythonDirectory, inSystemPath))
         return true;
   }

   if (isPythonIn(boost::filesystem::path(), pythonDirectory, inSystemPath))// Empty directory to search in system path
      return true;

   YADOMS_LOG(error) << "Python 3 executable not found";
   return false;
}

bool CPythonExecutable::isPythonIn(const boost::filesystem::path& directory,
                                   boost::filesystem::path& pythonDirectory,
                                   bool& inSystemPath)
{
   static const std::string ExpectedVersionString("Python 3.7");
   const std::string version = readPythonVersion(directory);

	if (!version.empty() && version.find(ExpectedVersionString) != std::string::npos)
   {
      YADOMS_LOG(information) << "Python 3 executable found in " << (directory.empty() ? "the system path" : directory.string());
      pythonDirectory = directory;
      inSystemPath = directory.empty();
      return true;
   }
   YADOMS_LOG(information) << "Python 3 executable not found in " << (directory.empty() ? "the system path" : directory.string());
   return false;
}

std::string CPythonExecutable::readPythonVersion(const boost::filesystem::path& initialDirectory)
{
   try
   {
      const auto& command = initialDirectory / shared::CExecutable::ToFileName(CPythonExecutablePath::getExecutableName());

      Poco::Process::Args args;
      args.push_back("--version");

      Poco::Pipe outPipe;
      auto processHandle = Poco::Process::launch(command.string(), args, nullptr, &outPipe, &outPipe);
      processHandle.wait();

      Poco::PipeInputStream iStr(outPipe);
      std::ostringstream oStr;
      Poco::StreamCopier::copyStream(iStr, oStr);
      return oStr.str();
   }
   catch (Poco::Exception& ex)
   {
      YADOMS_LOG(information) << "Unable to read Python 3 version, " << ex.what();
      return std::string();
   }
}

