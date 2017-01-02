#include "stdafx.h"
#include "PythonExecutable.h"
#include "PythonExecutablePath.h"
#include <shared/Executable.h>

CPythonExecutable::CPythonExecutable()
   : m_inSystemPath(false),
     m_found(findPythonDirectory(m_executableDirectory, m_inSystemPath)),
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
   return shared::CExecutable::ToFileName("python");
}

bool CPythonExecutable::findPythonDirectory(boost::filesystem::path& pythonDirectory,
                                            bool& inSystemPath)
{
   if (isPythonIn(boost::filesystem::path(), pythonDirectory, inSystemPath))// Empty directory to search in system path
      return true;

   //retrieve common path for the system
   std::vector<boost::filesystem::path> commonPaths;
   CPythonExecutablePath::getCommonPythonPaths(commonPaths);

   std::vector<boost::filesystem::path>::iterator i;
   for (i = commonPaths.begin(); i != commonPaths.end(); ++i)
   {
      if (isPythonIn(*i, pythonDirectory, inSystemPath))
         return true;
   }

   std::cerr << "Python executable not found" << std::endl;
   return false;
}

bool CPythonExecutable::isPythonIn(const boost::filesystem::path& directory,
                                   boost::filesystem::path& pythonDirectory,
                                   bool& inSystemPath)
{
   if (!readPythonVersion(directory).empty())
   {
      std::cout << "Python executable found in " << (directory.empty() ? "the system path" : directory.string()) << std::endl;
      pythonDirectory = directory;
      inSystemPath = directory.empty();
      return true;
   }
   std::cout << "Python executable not found in " << (directory.empty() ? "the system path" : directory.string()) << std::endl;
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
      Poco::ProcessHandle processHandle = Poco::Process::launch(command.string(), args, nullptr, &outPipe, &outPipe);
      processHandle.wait();

      Poco::PipeInputStream iStr(outPipe);
      std::ostringstream oStr;
      Poco::StreamCopier::copyStream(iStr, oStr);
      return oStr.str();
   }
   catch (Poco::Exception& ex)
   {
      std::cout << "Unable to read Python version, " << ex.what() << std::endl;
      return std::string();
   }
}

